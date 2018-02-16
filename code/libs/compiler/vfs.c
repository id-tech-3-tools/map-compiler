/*
   Copyright (c) 2001, Loki software, inc.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

   Redistributions of source code must retain the above copyright notice, this list
   of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.

   Neither the name of Loki software nor the names of its contributors may be used
   to endorse or promote products derived from this software without specific prior
   written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
   DIRECT,INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//
// Rules:
//
// - Directories should be searched in the following order: ~/.q3a/baseq3,
//   install dir (/usr/local/games/quake3/baseq3) and cd_path (/mnt/cdrom/baseq3).
//
// - Pak files are searched first inside the directories.
// - Case insensitive.
// - Unix-style slashes (/) (windows is backwards .. everyone knows that)
//
// Leonardo Zide (leo@lokigames.com)
//

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "cmdlib.h"
#include "filematch.h"
#include "mathlib.h"
#include "inout.h"
#include "vfs.h"
#include "unzip.h"

#if GDEF_OS_WINDOWS
#include <msdirent.h>
#else
#include <dirent.h>
#endif

typedef struct
{
	char*   name;
	unz_s zipinfo;
	unzFile zipfile;
	size_t size;
} VFS_PAKFILE;

typedef struct vfs_slist_s vfs_slist_t;

struct vfs_slist_s {
	void *data;
	vfs_slist_t *next;
};

#define vfs_slist_next(slist) ((slist) ? (((vfs_slist_t *)(slist))->next) : NULL)

static vfs_slist_t* vfs_slist_last(vfs_slist_t *list)
{
	if (list)
	{
		while (list->next)
		{
			list = list->next;
		}
	}
	return list;
}

static vfs_slist_t* vfs_slist_append(vfs_slist_t *list, void *data)
{
	vfs_slist_t *newList = safe_malloc(sizeof(vfs_slist_t));
	newList->data = data;
	newList->next = NULL;

	if (list)
	{
		vfs_slist_t *lastItem = vfs_slist_last(list);
		lastItem->next = newList;
		return list;
	}

	return newList;
}

static vfs_slist_t* vfs_slist_remove(vfs_slist_t *list, void *data)
{
	vfs_slist_t *temp = list, *prev = NULL;
	while (temp)
	{
		if (temp->data == data)
		{
			if (prev)
			{
				prev->next = temp->next;
			}
			else
			{
				list = temp->next;
			}
			free(temp);
			break;
		}
		prev = temp;
		temp = prev->next;
	}

	return list;
}

static int vfs_slist_length(vfs_slist_t *list)
{
	int length = 1;
	vfs_slist_t *next;
	while ((next = list->next))
	{
		length++;
	}

	return length;
}

static char* vfs_string_down(char *string)
{
	if (!string)
	{
		return NULL;
	}

	int n = strlen(string);
	char *s = string;

	for (int i = 0; i < n; i++, s++)
	{
		if (isupper(*s))
		{
			*s = tolower(*s);
		}
	}

	return string;
}

// =============================================================================
// Global variables

//!\todo Define globally or use heap-allocated string.
#define NAME_MAX 255

static vfs_slist_t*  g_unzFiles;
static vfs_slist_t*  g_pakFiles;
static char g_strDirs[VFS_MAXDIRS][PATH_MAX + 1];
static int g_numDirs;
char g_strForbiddenDirs[VFS_MAXDIRS][PATH_MAX + 1];
int g_numForbiddenDirs = 0;
static qboolean g_bUsePak = qtrue;

// =============================================================================
// Static functions

static void vfsAddSlash(char *str) {
	int n = strlen(str);
	if (n > 0) {
		if (str[n - 1] != '\\' && str[n - 1] != '/') {
			strncat_s(str, NAME_MAX, "/", NAME_MAX);
		}
	}
}

static void vfsFixDOSName(char *src) {
	if (src == NULL) {
		return;
	}

	while (*src)
	{
		if (*src == '\\') {
			*src = '/';
		}
		src++;
	}
}

static void vfsInitPakFile(const char *filename) {
	unz_global_info gi;
	unzFile uf;
	unsigned int i;
	int err;

	uf = unzOpen(filename);
	if (uf == NULL) {
		return;
	}

	g_unzFiles = vfs_slist_append(g_unzFiles, uf);

	err = unzGetGlobalInfo(uf, &gi);
	if (err != UNZ_OK) {
		return;
	}
	unzGoToFirstFile(uf);

	for (i = 0; i < gi.number_entry; i++)
	{
		char filename_inzip[NAME_MAX];
		unz_file_info file_info;
		VFS_PAKFILE* file;

		err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
		if (err != UNZ_OK) {
			break;
		}

		file = (VFS_PAKFILE*)safe_malloc(sizeof(VFS_PAKFILE));
		g_pakFiles = vfs_slist_append(g_pakFiles, file);

		vfsFixDOSName(filename_inzip);
		vfs_string_down(filename_inzip);

		file->name = _strdup(filename_inzip);
		file->size = file_info.uncompressed_size;
		file->zipfile = uf;
		memcpy(&file->zipinfo, uf, sizeof(unz_s));

		if ((i + 1) < gi.number_entry) {
			err = unzGoToNextFile(uf);
			if (err != UNZ_OK) {
				break;
			}
		}
	}
}

// =============================================================================
// Global functions

// reads all pak files from a dir
void vfsInitDirectory(const char *path) {
	char filename[PATH_MAX];
	char *dirlist;
	int j;
	DIR *dir;
	dirent *dp;

	for (j = 0; j < g_numForbiddenDirs; ++j)
	{
		char* dbuf = _strdup(path);
		if (*dbuf && dbuf[strlen(dbuf) - 1] == '/') {
			dbuf[strlen(dbuf) - 1] = 0;
		}
		const char *p = strrchr(dbuf, '/');
		p = (p ? (p + 1) : dbuf);
		if (matchpattern(p, g_strForbiddenDirs[j], TRUE)) {
			free(dbuf);
			break;
		}
		free(dbuf);
	}
	if (j < g_numForbiddenDirs) {
		return;
	}

	if (g_numDirs == VFS_MAXDIRS) {
		return;
	}

	Sys_Printf("VFS Init: %s\n", path);

	strncpy_s(g_strDirs[g_numDirs], PATH_MAX, path, PATH_MAX);
	g_strDirs[g_numDirs][PATH_MAX] = 0;
	vfsFixDOSName(g_strDirs[g_numDirs]);
	vfsAddSlash(g_strDirs[g_numDirs]);
	g_numDirs++;

	if (g_bUsePak) {
		dir = opendir(path);

		if (dir != NULL) {
			while (1)
			{
				dp = readdir(dir);
				const char* name = dp->d_name;
				if (name == NULL) {
					break;
				}

				for (j = 0; j < g_numForbiddenDirs; ++j)
				{
					const char *p = strrchr(name, '/');
					p = (p ? (p + 1) : name);
					if (matchpattern(p, g_strForbiddenDirs[j], TRUE)) {
						break;
					}
				}
				if (j < g_numForbiddenDirs) {
					continue;
				}

				dirlist = _strdup(name);

				{
					char *ext = strrchr(dirlist, '.');

					if (ext && !Q_stricmp(ext, ".pk3dir")) {
						if (g_numDirs == VFS_MAXDIRS) {
							continue;
						}
						snprintf(g_strDirs[g_numDirs], PATH_MAX, "%s/%s", path, name);
						g_strDirs[g_numDirs][PATH_MAX] = '\0';
						vfsFixDOSName(g_strDirs[g_numDirs]);
						vfsAddSlash(g_strDirs[g_numDirs]);
						++g_numDirs;
					}

					if ((ext == NULL) || (Q_stricmp(ext, ".pk3") != 0)) {
						continue;
					}
				}

				sprintf_s(filename, NAME_MAX, "%s/%s", path, dirlist);
				vfsInitPakFile(filename);

				free(dirlist);
			}
			closedir(dir);
		}
	}
}

// frees all memory that we allocated
void vfsShutdown() {
	while (g_unzFiles)
	{
		unzClose((unzFile)g_unzFiles->data);
		g_unzFiles = vfs_slist_remove(g_unzFiles, g_unzFiles->data);
	}

	while (g_pakFiles)
	{
		VFS_PAKFILE* file = (VFS_PAKFILE*)g_pakFiles->data;
		free(file->name);
		free(file);
		g_pakFiles = vfs_slist_remove(g_pakFiles, file);
	}
}

// return the number of files that match
int vfsGetFileCount(const char *filename) {
	int count = 0;
	char fixed[NAME_MAX], tmp[NAME_MAX];
	vfs_slist_t *lst;

	strncpy_s(fixed, NAME_MAX, filename, NAME_MAX);
	vfsFixDOSName(fixed);
	vfs_string_down(fixed);

	for (lst = g_pakFiles; lst != NULL; lst = vfs_slist_next(lst))
	{
		VFS_PAKFILE* file = (VFS_PAKFILE*)lst->data;

		if (strcmp(file->name, fixed) == 0) {
			count++;
		}
	}

	for (int i = 0; i < g_numDirs; i++)
	{
		strncpy_s(tmp, NAME_MAX, g_strDirs[i], NAME_MAX);
		strncat_s(tmp, NAME_MAX, fixed, NAME_MAX);
		if (_access(tmp, R_OK) == 0) {
			count++;
		}
	}

	return count;
}

// NOTE: when loading a file, you have to allocate one extra byte and set it to \0
int vfsLoadFile(const char *filename, void **bufferptr, int index) {
	int i, count = 0;
	char tmp[NAME_MAX], fixed[NAME_MAX];
	vfs_slist_t *lst;

	// filename is a full path
	if (index == -1) {
		long len;
		FILE *f;

		f = fopen(filename, "rb");
		if (f == NULL) {
			return -1;
		}

		fseek(f, 0, SEEK_END);
		len = ftell(f);
		rewind(f);

		*bufferptr = safe_malloc(len + 1);
		if (*bufferptr == NULL) {
			fclose(f);
			return -1;
		}

		if (fread(*bufferptr, 1, len, f) != (size_t)len) {
			fclose(f);
			return -1;
		}
		fclose(f);

		// we need to end the buffer with a 0
		((char*)(*bufferptr))[len] = 0;

		return len;
	}

	*bufferptr = NULL;
	strncpy_s(fixed, NAME_MAX, filename, NAME_MAX);
	vfsFixDOSName(fixed);
	vfs_string_down(fixed);

	for (i = 0; i < g_numDirs; i++)
	{
		strncpy_s(tmp, NAME_MAX, g_strDirs[i], NAME_MAX);
		strncat_s(tmp, NAME_MAX, filename, NAME_MAX);
		if (_access(tmp, R_OK) == 0) {
			if (count == index) {
				long len;
				FILE *f;

				f = fopen(tmp, "rb");
				if (f == NULL) {
					return -1;
				}

				fseek(f, 0, SEEK_END);
				len = ftell(f);
				rewind(f);

				*bufferptr = safe_malloc(len + 1);
				if (*bufferptr == NULL) {
					fclose(f);
					return -1;
				}

				if (fread(*bufferptr, 1, len, f) != (size_t)len) {
					fclose(f);
					return -1;
				}
				fclose(f);

				// we need to end the buffer with a 0
				((char*)(*bufferptr))[len] = 0;

				return len;
			}

			count++;
		}
	}

	for (lst = g_pakFiles; lst != NULL; lst = vfs_slist_next(lst))
	{
		VFS_PAKFILE* file = (VFS_PAKFILE*)lst->data;

		if (strcmp(file->name, fixed) != 0) {
			continue;
		}

		if (count == index) {
			memcpy(file->zipfile, &file->zipinfo, sizeof(unz_s));

			if (unzOpenCurrentFile(file->zipfile) != UNZ_OK) {
				return -1;
			}

			*bufferptr = safe_malloc(file->size + 1);
			// we need to end the buffer with a 0
			((char*)(*bufferptr))[file->size] = 0;

			i = unzReadCurrentFile(file->zipfile, *bufferptr, file->size);
			unzCloseCurrentFile(file->zipfile);
			if (i < 0) {
				return -1;
			}
			else {
				return file->size;
			}
		}

		count++;
	}

	return -1;
}
