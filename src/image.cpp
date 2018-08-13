/* -------------------------------------------------------------------------------

   Copyright (C) 1999-2007 id Software, Inc. and contributors.
   For a list of contributors, see the accompanying CONTRIBUTORS file.

   This file is part of GtkRadiant.

   GtkRadiant is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GtkRadiant is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GtkRadiant; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

   ----------------------------------------------------------------------------------

   This code has been altered significantly from its original form, to support
   several games based on the Quake III Arena engine, in the form of "Q3Map2."

   ------------------------------------------------------------------------------- */



/* marker */
#define IMAGE_C

#include "lodepng.h"
#include "jpgd.h"

/* dependencies */
#include "q3map2.h"

/* -------------------------------------------------------------------------------

   this file contains image pool management with reference counting. note: it isn't
   reentrant, so only call it from init/shutdown code or wrap calls in a mutex

   ------------------------------------------------------------------------------- */

/*
   LoadDDSBuffer()
   loads a dxtc (1, 3, 5) dds buffer into a valid rgba image
 */

static void LoadDDSBuffer( byte *buffer, int size, byte **pixels, int *width, int *height ){
	int w, h;
	ddsPF_t pf;


	/* dummy check */
	if ( buffer == NULL || size <= 0 || pixels == NULL || width == NULL || height == NULL ) {
		return;
	}

	/* null out */
	*pixels = 0;
	*width = 0;
	*height = 0;

	/* get dds info */
	if ( DDSGetInfo( (ddsBuffer_t*) buffer, &w, &h, &pf ) ) {
		Sys_FPrintf( SYS_WRN, "WARNING: Invalid DDS texture\n" );
		return;
	}

	/* only certain types of dds textures are supported */
	if ( pf != DDS_PF_ARGB8888 && pf != DDS_PF_DXT1 && pf != DDS_PF_DXT3 && pf != DDS_PF_DXT5 ) {
		Sys_FPrintf( SYS_WRN, "WARNING: Only DDS texture formats ARGB8888, DXT1, DXT3, and DXT5 are supported (%d)\n", pf );
		return;
	}

	/* create image pixel buffer */
	*width = w;
	*height = h;
	*pixels = static_cast<byte*>(safe_malloc(w * h * 4));

	/* decompress the dds texture */
	DDSDecompress( (ddsBuffer_t*) buffer, *pixels );
}

/*
   LoadPNGBuffer()
   loads a png file buffer into a valid rgba image
 */
static void LoadPNGBuffer(byte *buffer, int size, byte **pixels, int *width, int *height)
{
	LodePNGState state;
	state.info_raw.colortype = LCT_RGBA;
	state.info_raw.bitdepth = 8;
	lodepng_state_init(&state);
	unsigned error = lodepng_decode(pixels, (unsigned *)width, (unsigned *)height, &state, buffer, size);
	if (error) {
		Sys_FPrintf(SYS_WRN, "WARNING: An error occurred reading PNG image: %s\n", lodepng_error_text(error));
	}
	lodepng_state_cleanup(&state);
}

/*
	LoadJPGBuff()
	loads a jpg file buffer into a valid rgba image
*/
int LoadJPGBuff(void *src_buffer, int src_size, unsigned char **pic, int *width, int *height) {
	int actual_comps = 0;
	unsigned char *output = jpgd::decompress_jpeg_image_from_memory(
		static_cast<unsigned char*>(src_buffer), src_size, width, height, &actual_comps, 4
	);
	if (output == NULL)
	{
		// TODO: handle actual error codes
		*pic = 0;
		return -1;
	}
	*pic = output;
	return 1;
}

/*
   ImageInit()
   implicitly called by every function to set up image list
 */

static void ImageInit( void ){
	int i;


	if ( numImages <= 0 ) {
		/* clear images (fixme: this could theoretically leak) */
		memset( images, 0, sizeof( images ) );

		/* generate *bogus image */
		images[ 0 ].name = static_cast<char*>(safe_malloc(strlen(DEFAULT_IMAGE) + 1));
		strcpy( images[ 0 ].name, DEFAULT_IMAGE );
		images[ 0 ].filename = static_cast<char*>(safe_malloc(strlen(DEFAULT_IMAGE) + 1));
		strcpy( images[ 0 ].filename, DEFAULT_IMAGE );
		images[ 0 ].width = 64;
		images[ 0 ].height = 64;
		images[ 0 ].refCount = 1;
		images[ 0 ].pixels = static_cast<byte*>(safe_malloc(64 * 64 * 4));
		for ( i = 0; i < ( 64 * 64 * 4 ); i++ )
			images[ 0 ].pixels[ i ] = 255;
	}
}



/*
   ImageFree()
   frees an rgba image
 */

void ImageFree( image_t *image ){
	/* dummy check */
	if ( image == NULL ) {
		return;
	}

	/* decrement refcount */
	image->refCount--;

	/* free? */
	if ( image->refCount <= 0 ) {
		if ( image->name != NULL ) {
			free( image->name );
		}
		image->name = NULL;
		if ( image->filename != NULL ) {
			free( image->filename );
		}
		image->filename = NULL;
		free( image->pixels );
		image->width = 0;
		image->height = 0;
		numImages--;
	}
}



/*
   ImageFind()
   finds an existing rgba image and returns a pointer to the image_t struct or NULL if not found
 */

image_t *ImageFind( const char *filename ){
	int i;
	char name[ 1024 ];


	/* init */
	ImageInit();

	/* dummy check */
	if ( filename == NULL || filename[ 0 ] == '\0' ) {
		return NULL;
	}

	/* strip file extension off name */
	strcpy( name, filename );
	StripExtension( name );

	/* search list */
	for ( i = 0; i < MAX_IMAGES; i++ )
	{
		if ( images[ i ].name != NULL && !strcmp( name, images[ i ].name ) ) {
			return &images[ i ];
		}
	}

	/* no matching image found */
	return NULL;
}



/*
   ImageLoad()
   loads an rgba image and returns a pointer to the image_t struct or NULL if not found
 */

image_t *ImageLoad( const char *filename ){
	int i;
	image_t     *image;
	char name[ 1024 ];
	int size;
	byte        *buffer = NULL;
	qboolean alphaHack = qfalse;


	/* init */
	ImageInit();

	/* dummy check */
	if ( filename == NULL || filename[ 0 ] == '\0' ) {
		return NULL;
	}

	/* strip file extension off name */
	strcpy( name, filename );
	StripExtension( name );

	/* try to find existing image */
	image = ImageFind( name );
	if ( image != NULL ) {
		image->refCount++;
		return image;
	}

	/* none found, so find first non-null image */
	image = NULL;
	for ( i = 0; i < MAX_IMAGES; i++ )
	{
		if ( images[ i ].name == NULL ) {
			image = &images[ i ];
			break;
		}
	}

	/* too many images? */
	if ( image == NULL ) {
		Error( "MAX_IMAGES (%d) exceeded, there are too many image files referenced by the map.", MAX_IMAGES );
	}

	/* set it up */
	image->name = static_cast<char*>(safe_malloc(strlen(name) + 1));
	strcpy( image->name, name );

	/* attempt to load tga */
	StripExtension( name );
	strcat( name, ".tga" );
	size = g_vfs.load(name, (void**) &buffer);
	if ( size > 0 ) {
		LoadTGABuffer( buffer, buffer + size, &image->pixels, &image->width, &image->height );
	}
	else
	{
		/* attempt to load png */
		StripExtension( name );
		strcat( name, ".png" );
		size = g_vfs.load(name, (void**) &buffer);
		if ( size > 0 ) {
			LoadPNGBuffer( buffer, size, &image->pixels, &image->width, &image->height );
		}
		else
		{
			/* attempt to load jpg */
			StripExtension( name );
			strcat( name, ".jpg" );
			size = g_vfs.load(name, (void**)&buffer);
			if ( size > 0 ) {
				if ( LoadJPGBuff( buffer, size, &image->pixels, &image->width, &image->height ) == -1 && image->pixels != NULL ) {
					// On error, LoadJPGBuff might store a pointer to the error message in image->pixels
					Sys_FPrintf( SYS_WRN, "WARNING: LoadJPGBuff: %s\n", (unsigned char*) image->pixels );
				}
				alphaHack = qtrue;
			}
			else
			{
				/* attempt to load dds */
				StripExtension( name );
				strcat( name, ".dds" );
				size = g_vfs.load(name, (void**)&buffer);
				if ( size > 0 ) {
					LoadDDSBuffer( buffer, size, &image->pixels, &image->width, &image->height );

					/* debug code */
					#if 1
					{
						ddsPF_t pf;
						DDSGetInfo( (ddsBuffer_t*) buffer, NULL, NULL, &pf );
						Sys_Printf( "pf = %d\n", pf );
						if ( image->width > 0 ) {
							StripExtension( name );
							strcat( name, "_converted.tga" );
							WriteTGA( "C:\\games\\quake3\\baseq3\\textures\\rad\\dds_converted.tga", image->pixels, image->width, image->height );
						}
					}
					#endif
				}
				else
				{
					/* attempt to load ktx */
					StripExtension( name );
					strcat( name, ".ktx" );
					size = g_vfs.load(name, (void**)&buffer);
					if ( size > 0 ) {
						LoadKTXBufferFirstImage( buffer, size, &image->pixels, &image->width, &image->height );
					}
				}
			}
		}
	}

	/* free file buffer */
	free( buffer );

	/* make sure everything's kosher */
	if ( size <= 0 || image->width <= 0 || image->height <= 0 || image->pixels == NULL ) {
		//%	Sys_Printf( "size = %d  width = %d  height = %d  pixels = 0x%08x (%s)\n",
		//%		size, image->width, image->height, image->pixels, name );
		free( image->name );
		image->name = NULL;
		return NULL;
	}

	/* set filename */
	image->filename = static_cast<char*>(safe_malloc(strlen(name) + 1));
	strcpy( image->filename, name );

	/* set count */
	image->refCount = 1;
	numImages++;

	if ( alphaHack ) {
		StripExtension( name );
		strcat( name, "_alpha.jpg" );
		size = g_vfs.load(name, (void**)&buffer);
		if ( size > 0 ) {
			unsigned char *pixels;
			int width, height;
			if ( LoadJPGBuff( buffer, size, &pixels, &width, &height ) == -1 ) {
				if (pixels) {
					// On error, LoadJPGBuff might store a pointer to the error message in pixels
					Sys_FPrintf( SYS_WRN, "WARNING: LoadJPGBuff %s %s\n", name, (unsigned char*) pixels );
				}				
			} else {
				if ( width == image->width && height == image->height ) {
					int i;
					for ( i = 0; i < width * height; ++i )
						image->pixels[4 * i + 3] = pixels[4 * i + 2];  // copy alpha from blue channel
				}
				free( pixels );
			}
			free( buffer );
		}
	}

	/* return the image */
	return image;
}
