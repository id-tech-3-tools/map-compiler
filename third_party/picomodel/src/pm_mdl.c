/* -----------------------------------------------------------------------------
  MIT License

  Copyright (c) 2018 ryven.mt@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
----------------------------------------------------------------------------- */

/* dependencies */
#include "picointernal.h"
#include <string.h>

#define MDL_NUMVERTEXNORMALS 162

float mdl_normals[MDL_NUMVERTEXNORMALS][3] =
{
	{ -0.525731f, 0.000000f, 0.850651f },
{ -0.442863f, 0.238856f, 0.864188f },
{ -0.295242f, 0.000000f, 0.955423f },
{ -0.309017f, 0.500000f, 0.809017f },
{ -0.162460f, 0.262866f, 0.951056f },
{ 0.000000f, 0.000000f, 1.000000f },
{ 0.000000f, 0.850651f, 0.525731f },
{ -0.147621f, 0.716567f, 0.681718f },
{ 0.147621f, 0.716567f, 0.681718f },
{ 0.000000f, 0.525731f, 0.850651f },
{ 0.309017f, 0.500000f, 0.809017f },
{ 0.525731f, 0.000000f, 0.850651f },
{ 0.295242f, 0.000000f, 0.955423f },
{ 0.442863f, 0.238856f, 0.864188f },
{ 0.162460f, 0.262866f, 0.951056f },
{ -0.681718f, 0.147621f, 0.716567f },
{ -0.809017f, 0.309017f, 0.500000f },
{ -0.587785f, 0.425325f, 0.688191f },
{ -0.850651f, 0.525731f, 0.000000f },
{ -0.864188f, 0.442863f, 0.238856f },
{ -0.716567f, 0.681718f, 0.147621f },
{ -0.688191f, 0.587785f, 0.425325f },
{ -0.500000f, 0.809017f, 0.309017f },
{ -0.238856f, 0.864188f, 0.442863f },
{ -0.425325f, 0.688191f, 0.587785f },
{ -0.716567f, 0.681718f, -0.147621f },
{ -0.500000f, 0.809017f, -0.309017f },
{ -0.525731f, 0.850651f, 0.000000f },
{ 0.000000f, 0.850651f, -0.525731f },
{ -0.238856f, 0.864188f, -0.442863f },
{ 0.000000f, 0.955423f, -0.295242f },
{ -0.262866f, 0.951056f, -0.162460f },
{ 0.000000f, 1.000000f, 0.000000f },
{ 0.000000f, 0.955423f, 0.295242f },
{ -0.262866f, 0.951056f, 0.162460f },
{ 0.238856f, 0.864188f, 0.442863f },
{ 0.262866f, 0.951056f, 0.162460f },
{ 0.500000f, 0.809017f, 0.309017f },
{ 0.238856f, 0.864188f, -0.442863f },
{ 0.262866f, 0.951056f, -0.162460f },
{ 0.500000f, 0.809017f, -0.309017f },
{ 0.850651f, 0.525731f, 0.000000f },
{ 0.716567f, 0.681718f, 0.147621f },
{ 0.716567f, 0.681718f, -0.147621f },
{ 0.525731f, 0.850651f, 0.000000f },
{ 0.425325f, 0.688191f, 0.587785f },
{ 0.864188f, 0.442863f, 0.238856f },
{ 0.688191f, 0.587785f, 0.425325f },
{ 0.809017f, 0.309017f, 0.500000f },
{ 0.681718f, 0.147621f, 0.716567f },
{ 0.587785f, 0.425325f, 0.688191f },
{ 0.955423f, 0.295242f, 0.000000f },
{ 1.000000f, 0.000000f, 0.000000f },
{ 0.951056f, 0.162460f, 0.262866f },
{ 0.850651f, -0.525731f, 0.000000f },
{ 0.955423f, -0.295242f, 0.000000f },
{ 0.864188f, -0.442863f, 0.238856f },
{ 0.951056f, -0.162460f, 0.262866f },
{ 0.809017f, -0.309017f, 0.500000f },
{ 0.681718f, -0.147621f, 0.716567f },
{ 0.850651f, 0.000000f, 0.525731f },
{ 0.864188f, 0.442863f, -0.238856f },
{ 0.809017f, 0.309017f, -0.500000f },
{ 0.951056f, 0.162460f, -0.262866f },
{ 0.525731f, 0.000000f, -0.850651f },
{ 0.681718f, 0.147621f, -0.716567f },
{ 0.681718f, -0.147621f, -0.716567f },
{ 0.850651f, 0.000000f, -0.525731f },
{ 0.809017f, -0.309017f, -0.500000f },
{ 0.864188f, -0.442863f, -0.238856f },
{ 0.951056f, -0.162460f, -0.262866f },
{ 0.147621f, 0.716567f, -0.681718f },
{ 0.309017f, 0.500000f, -0.809017f },
{ 0.425325f, 0.688191f, -0.587785f },
{ 0.442863f, 0.238856f, -0.864188f },
{ 0.587785f, 0.425325f, -0.688191f },
{ 0.688191f, 0.587785f, -0.425325f },
{ -0.147621f, 0.716567f, -0.681718f },
{ -0.309017f, 0.500000f, -0.809017f },
{ 0.000000f, 0.525731f, -0.850651f },
{ -0.525731f, 0.000000f, -0.850651f },
{ -0.442863f, 0.238856f, -0.864188f },
{ -0.295242f, 0.000000f, -0.955423f },
{ -0.162460f, 0.262866f, -0.951056f },
{ 0.000000f, 0.000000f, -1.000000f },
{ 0.295242f, 0.000000f, -0.955423f },
{ 0.162460f, 0.262866f, -0.951056f },
{ -0.442863f, -0.238856f, -0.864188f },
{ -0.309017f, -0.500000f, -0.809017f },
{ -0.162460f, -0.262866f, -0.951056f },
{ 0.000000f, -0.850651f, -0.525731f },
{ -0.147621f, -0.716567f, -0.681718f },
{ 0.147621f, -0.716567f, -0.681718f },
{ 0.000000f, -0.525731f, -0.850651f },
{ 0.309017f, -0.500000f, -0.809017f },
{ 0.442863f, -0.238856f, -0.864188f },
{ 0.162460f, -0.262866f, -0.951056f },
{ 0.238856f, -0.864188f, -0.442863f },
{ 0.500000f, -0.809017f, -0.309017f },
{ 0.425325f, -0.688191f, -0.587785f },
{ 0.716567f, -0.681718f, -0.147621f },
{ 0.688191f, -0.587785f, -0.425325f },
{ 0.587785f, -0.425325f, -0.688191f },
{ 0.000000f, -0.955423f, -0.295242f },
{ 0.000000f, -1.000000f, 0.000000f },
{ 0.262866f, -0.951056f, -0.162460f },
{ 0.000000f, -0.850651f, 0.525731f },
{ 0.000000f, -0.955423f, 0.295242f },
{ 0.238856f, -0.864188f, 0.442863f },
{ 0.262866f, -0.951056f, 0.162460f },
{ 0.500000f, -0.809017f, 0.309017f },
{ 0.716567f, -0.681718f, 0.147621f },
{ 0.525731f, -0.850651f, 0.000000f },
{ -0.238856f, -0.864188f, -0.442863f },
{ -0.500000f, -0.809017f, -0.309017f },
{ -0.262866f, -0.951056f, -0.162460f },
{ -0.850651f, -0.525731f, 0.000000f },
{ -0.716567f, -0.681718f, -0.147621f },
{ -0.716567f, -0.681718f, 0.147621f },
{ -0.525731f, -0.850651f, 0.000000f },
{ -0.500000f, -0.809017f, 0.309017f },
{ -0.238856f, -0.864188f, 0.442863f },
{ -0.262866f, -0.951056f, 0.162460f },
{ -0.864188f, -0.442863f, 0.238856f },
{ -0.809017f, -0.309017f, 0.500000f },
{ -0.688191f, -0.587785f, 0.425325f },
{ -0.681718f, -0.147621f, 0.716567f },
{ -0.442863f, -0.238856f, 0.864188f },
{ -0.587785f, -0.425325f, 0.688191f },
{ -0.309017f, -0.500000f, 0.809017f },
{ -0.147621f, -0.716567f, 0.681718f },
{ -0.425325f, -0.688191f, 0.587785f },
{ -0.162460f, -0.262866f, 0.951056f },
{ 0.442863f, -0.238856f, 0.864188f },
{ 0.162460f, -0.262866f, 0.951056f },
{ 0.309017f, -0.500000f, 0.809017f },
{ 0.147621f, -0.716567f, 0.681718f },
{ 0.000000f, -0.525731f, 0.850651f },
{ 0.425325f, -0.688191f, 0.587785f },
{ 0.587785f, -0.425325f, 0.688191f },
{ 0.688191f, -0.587785f, 0.425325f },
{ -0.955423f, 0.295242f, 0.000000f },
{ -0.951056f, 0.162460f, 0.262866f },
{ -1.000000f, 0.000000f, 0.000000f },
{ -0.850651f, 0.000000f, 0.525731f },
{ -0.955423f, -0.295242f, 0.000000f },
{ -0.951056f, -0.162460f, 0.262866f },
{ -0.864188f, 0.442863f, -0.238856f },
{ -0.951056f, 0.162460f, -0.262866f },
{ -0.809017f, 0.309017f, -0.500000f },
{ -0.864188f, -0.442863f, -0.238856f },
{ -0.951056f, -0.162460f, -0.262866f },
{ -0.809017f, -0.309017f, -0.500000f },
{ -0.681718f, 0.147621f, -0.716567f },
{ -0.681718f, -0.147621f, -0.716567f },
{ -0.850651f, 0.000000f, -0.525731f },
{ -0.688191f, 0.587785f, -0.425325f },
{ -0.587785f, 0.425325f, -0.688191f },
{ -0.425325f, 0.688191f, -0.587785f },
{ -0.425325f, -0.688191f, -0.587785f },
{ -0.587785f, -0.425325f, -0.688191f },
{ -0.688191f, -0.587785f, -0.425325f },
};

const char *MDL_IDENT = "IDPO";
const int MDL_VERSION = 6;

#define MAX_LBM_HEIGHT 480
#define MAXALIASVERTS 2000

typedef float vec3_t[3];

typedef struct {
	int			ident;
	int			version;
	vec3_t		scale;
	vec3_t		scaleOrigin;
	float		boundingRadius;
	vec3_t		eyePosition;
	int			numSkins;
	int			skinWidth;
	int			skinHeight;
	int			numVerts;
	int			numTris;
	int			numFrames;
	int	        synctype;
	int			flags;
	float		size;
} mdl_header_t;

#define MDL_SKIN_SINGLE 0
#define MDL_SKIN_GROUP 1

#define MDL_FRAME_SINGLE 0
#define MDL_FRAME_GROUP 1

/* Texture coords */
typedef struct
{
	int onseam;
	int s;
	int t;
} mdl_texcoord_t;

/* Triangle info */
typedef struct
{
	int facesfront; /* 0 = backface, 1 = frontface */
	int vertex[3]; /* vertex indices */
} mdl_triangle_t;

/* Compressed vertex */
typedef struct
{
	unsigned char v[3];
	unsigned char normalIndex;
} mdl_vertex_t;

/* Skin */
typedef struct
{
	int group; /* 0 = single, 1 = group */
} mdl_skin_type_t;

/* Skin */
typedef struct
{
	int group; /* 0 = single, 1 = group */
	picoByte_t *data; /* texture data */
} mdl_skin_t;

/* Group of pictures */
typedef struct
{
	int group; /* 1 = group */
	int numTextures; /* number of pics */
	float *time; /* time duration for each pic */
	picoByte_t **data; /* texture data */
} mdl_groupskin_t;

/* Simple frame description */
typedef struct
{
	mdl_vertex_t bboxmin; /* bouding box min */
	mdl_vertex_t bboxmax; /* bouding box max */
	char name[16];
} mdl_simpleframedesc_t;

/* Simple frame */
typedef struct
{
	mdl_vertex_t bboxmin; /* bouding box min */
	mdl_vertex_t bboxmax; /* bouding box max */
	char name[16];
	mdl_vertex_t *verts; /* vertex list of the frame */
} mdl_simpleframe_t;

/* Model frame */
typedef struct
{
	int type; /* 0 = simple, !0 = group */
	mdl_simpleframe_t frame;
} mdl_frame_t;

/* Group of simple frames */
typedef struct
{
	int numFrames;
	mdl_vertex_t min; /* min pos in all simple frames */
	mdl_vertex_t max; /* max pos in all simple frames */
} mdl_groupframedesc_t;

/* Group of simple frames */
typedef struct
{
	int type; /* !0 = group */
	int numFrames; /* number of frames in the group */
	mdl_vertex_t min; /* min pos in all simple frames */
	mdl_vertex_t max; /* max pos in all simple frames */
	float *time; /* time duration for each frame */
	mdl_simpleframe_t *frames; /* simple frame list */
} mdl_groupframe_t;

/*
_mdl_canload()
validates a quake mdl model file.
*/
static int _mdl_canload(PM_PARAMS_CANLOAD) {
	const mdl_header_t *mdl;

	/* sanity check */
	if ((size_t)bufSize < (sizeof(*mdl) * 2)) {
		return PICO_PMV_ERROR_SIZE;
	}

	/* set as mdl */
	mdl = (const mdl_header_t*)buffer;

	/* check mdl magic */
	if (mdl->ident != *((const int*)MDL_IDENT)) {
		return PICO_PMV_ERROR_IDENT;
	}

	/* check mdl version */
	if (_pico_little_long(mdl->version) != MDL_VERSION) {
		return PICO_PMV_ERROR_VERSION;
	}

	/* file seems to be a valid mdl */
	return PICO_PMV_OK;
}

static void* _offset_to(void **buf, size_t size) {
	char **ptr = (char **)buf;
	char *before = (char *)*buf;
	*ptr += size;
	return before;
}

static char* _make_texture_path(const char *in, char *out) {
	memcpy(out, in, 256);
	_pico_setfext(out, "");
	strcat(out, "_img");
	_pico_unixify(out);
	out[255] = '\0';
	return out;
}

/*
_mdl_load()
loads a quake mdl model file.
*/
static picoModel_t *_mdl_load(PM_PARAMS_LOAD) {
	picoModel_t *picoModel;
	picoSurface_t *picoSurface;
	picoShader_t *picoShader;
	mdl_header_t *mdlHeader;
	picoByte_t *ptr, *buff;
	picoVec2_t st;
	picoVec3_t xyz;
	mdl_skin_t *skin;
	mdl_texcoord_t *ofsSt;
	mdl_triangle_t *ofsTriangles;
	mdl_vertex_t *ofsVerts = NULL;
	mdl_frame_t *frame;
	int i;
	char texturePath[256];

	/* -------------------------------------------------
	mdl loading
	------------------------------------------------- */

	/* set as mdl */
	buff = ptr = (picoByte_t*)_pico_alloc(bufSize);
	memcpy(ptr, buffer, bufSize);
	mdlHeader = (mdl_header_t*)ptr;

	/* check ident and version */
	if (mdlHeader->ident != *((int*)MDL_IDENT) || _pico_little_long(mdlHeader->version) != MDL_VERSION) {
		_pico_printf(PICO_ERROR, "%s is not an MDL File!", fileName);
		_pico_free(buff);
		return NULL;
	}

	/* swap header */
	mdlHeader->ident = _pico_little_long(mdlHeader->ident);
	mdlHeader->version = _pico_little_long(mdlHeader->version);
	for (i = 0; i < 3; i++) {
		mdlHeader->scale[i] = _pico_little_float(mdlHeader->scale[i]);
		mdlHeader->scaleOrigin[i] = _pico_little_float(mdlHeader->scaleOrigin[i]);
		mdlHeader->eyePosition[i] = _pico_little_float(mdlHeader->eyePosition[i]);
	}
	mdlHeader->boundingRadius = _pico_little_float(mdlHeader->boundingRadius);
	mdlHeader->numSkins = _pico_little_long(mdlHeader->numSkins);
	mdlHeader->skinWidth = _pico_little_long(mdlHeader->skinWidth);
	mdlHeader->skinHeight = _pico_little_long(mdlHeader->skinHeight);
	mdlHeader->numVerts = _pico_little_long(mdlHeader->numVerts);
	mdlHeader->numTris = _pico_little_long(mdlHeader->numTris);
	mdlHeader->numFrames = _pico_little_long(mdlHeader->numFrames);
	mdlHeader->synctype = _pico_little_long(mdlHeader->synctype);
	mdlHeader->flags = _pico_little_long(mdlHeader->flags);
	mdlHeader->size = _pico_little_float(mdlHeader->size);

	/* do sanity checks */
	if (mdlHeader->numFrames < 1) {
		_pico_printf(PICO_ERROR, "MDL with 0 frames");
		_pico_free(buff);
		return NULL;
	}

	if (frameNum < 0 || frameNum >= mdlHeader->numFrames) {
		_pico_printf(PICO_ERROR, "Invalid or out-of-range MDL frame specified");
		_pico_free(buff);
		return NULL;
	}

	if (mdlHeader->skinHeight > MAX_LBM_HEIGHT) {
		_pico_printf(PICO_ERROR, "Skin is taller than %d", MAX_LBM_HEIGHT);
	}

	if (mdlHeader->numVerts <= 0) {
		_pico_printf(PICO_ERROR, "MDL has no vertices");
	}

	if (mdlHeader->numVerts > MAXALIASVERTS) {
		_pico_printf(PICO_ERROR, "MDL has too many vertices, max is %d", MAXALIASVERTS);
	}

	if (mdlHeader->numTris <= 0) {
		_pico_printf(PICO_ERROR, "MDL has no triangles");
	}

	if (mdlHeader->skinWidth & 0x03) {
		_pico_printf(PICO_ERROR, "Skin width not multiple of 4");
	}

	if (mdlHeader->numSkins < 1) {
		_pico_printf(PICO_ERROR, "Invalid number of skins: %d", mdlHeader->numSkins);
	}

	/* -------------------------------------------------
	pico model creation
	------------------------------------------------- */

	/* create new pico model */
	picoModel = PicoNewModel();
	if (picoModel == NULL) {
		_pico_printf(PICO_ERROR, "Unable to allocate a new model");
		_pico_free(buff);
		return NULL;
	}

	/* do model setup */
	PicoSetModelFrameNum(picoModel, frameNum);
	PicoSetModelNumFrames(picoModel, mdlHeader->numFrames);
	PicoSetModelName(picoModel, fileName);
	PicoSetModelFileName(picoModel, fileName);

	/* Start parsing */
	_offset_to((void**)&ptr, sizeof(mdl_header_t));

	/* allocate new pico surface */
	picoSurface = PicoNewSurface(picoModel);
	if (picoSurface == NULL) {
		_pico_printf(PICO_ERROR, "Unable to allocate a new model surface");
		PicoFreeModel(picoModel);
		_pico_free(buff);
		return NULL;
	}

	/* md3 model surfaces are all triangle meshes */
	PicoSetSurfaceType(picoSurface, PICO_TRIANGLES);

	/* create new pico shader */
	picoShader = PicoNewShader(picoModel);
	if (picoShader == NULL) {
		_pico_printf(PICO_ERROR, "Unable to allocate a new model shader");
		PicoFreeModel(picoModel);
		_pico_free(buff);
		return NULL;
	}

	PicoSetShaderName(picoShader, _make_texture_path(fileName, texturePath));

	PicoSetSurfaceShader(picoSurface, picoShader);

	/* skip textures, for now */
	for (i = 0; i < mdlHeader->numSkins; i++) {
		skin = (mdl_skin_t*)ptr;
		if (_pico_little_long(skin->group) == MDL_SKIN_SINGLE) {
			_offset_to((void**)&ptr, sizeof(int)); // type
			_offset_to((void**)&ptr, mdlHeader->skinWidth * mdlHeader->skinHeight);
		}
		else {
			mdl_groupskin_t* groupSkins = (mdl_groupskin_t*)ptr;
			groupSkins->numTextures = _pico_little_long(groupSkins->numTextures);
			_offset_to((void**)&ptr, sizeof(int) * 2); // type, numTextures
			_offset_to((void**)&ptr, sizeof(float) * groupSkins->numTextures);
			_offset_to((void**)&ptr, mdlHeader->skinWidth * mdlHeader->skinHeight * groupSkins->numTextures);
		}
	}

	ofsSt = (mdl_texcoord_t*)_offset_to((void**)&ptr, sizeof(mdl_texcoord_t) * mdlHeader->numVerts);
	ofsTriangles = (mdl_triangle_t*)_offset_to((void**)&ptr, sizeof(mdl_triangle_t) * mdlHeader->numTris);

	/* find the right frame */
	for (i = 0; i < mdlHeader->numFrames; i++) {
		frame = (mdl_frame_t*)ptr;
		_offset_to((void**)&ptr, sizeof(int)); // type

		if (_pico_little_long(frame->type) == MDL_FRAME_SINGLE) {
			ofsVerts = (mdl_vertex_t*)(ptr + sizeof(mdl_simpleframedesc_t));
		}
		else {
			mdl_groupframedesc_t *desc = (mdl_groupframedesc_t *)_offset_to((void**)&ptr, sizeof(mdl_groupframedesc_t));
			_offset_to((void**)&ptr, sizeof(float) * _pico_little_long(desc->numFrames));
			ofsVerts = (mdl_vertex_t*)(ptr + sizeof(mdl_simpleframedesc_t));
		}

		/* skip frame data */
		_offset_to((void**)&ptr, sizeof(mdl_simpleframedesc_t) + sizeof(mdl_vertex_t) * mdlHeader->numVerts);

		/* target frame */
		if (i == frameNum) {
			break;
		}
	}

	/* add vertices, texture coordinates and normals */
	int iCurrent = 0;
	for (i = 0; i < mdlHeader->numTris; i++, ofsTriangles++) {
		int iTemp = iCurrent;
		/* add triangle vertices */
		for (int c = 0; c < 3; c++, iCurrent++)
		{
			int index = _pico_little_long(ofsTriangles->vertex[c]);
			mdl_vertex_t *vertex = &ofsVerts[index];
			mdl_texcoord_t *textCoord = &ofsSt[index];

			/* add vertex */
			xyz[0] = vertex->v[0] * mdlHeader->scale[0] + mdlHeader->scaleOrigin[0];
			xyz[1] = vertex->v[1] * mdlHeader->scale[1] + mdlHeader->scaleOrigin[1];
			xyz[2] = vertex->v[2] * mdlHeader->scale[2] + mdlHeader->scaleOrigin[2];
			PicoSetSurfaceXYZ(picoSurface, iCurrent, xyz);

			/* add texture coordinate */
			st[0] = _pico_little_long(textCoord->s);
			st[1] = _pico_little_long(textCoord->t);
			/* translate texture coordinate */
			if (_pico_little_long(ofsTriangles->facesfront) == 0 && _pico_little_long(textCoord->onseam) != 0) {
				st[0] += mdlHeader->skinWidth * 0.5f;
			}
			/* Scale s and t to range from 0.0 to 1.0 */
			st[0] = (st[0] + 0.5f) / mdlHeader->skinWidth;
			st[1] = 1.0f - (st[1] + 0.5f) / mdlHeader->skinHeight;
			PicoSetSurfaceST(picoSurface, 0, iCurrent, st);

			/* copy normal */
			PicoSetSurfaceNormal(picoSurface, iCurrent, mdl_normals[vertex->normalIndex]);
		}
		PicoSetSurfaceIndex(picoSurface, iTemp + 0, iTemp + 0);
		PicoSetSurfaceIndex(picoSurface, iTemp + 1, iTemp + 1);
		PicoSetSurfaceIndex(picoSurface, iTemp + 2, iTemp + 2);
	}

	_pico_free(buff);

	return picoModel;
}


/* pico file format module definition */
const picoModule_t picoModuleMDL =
{
	"1.0",                      /* module version string */
	"Quake Model Format",       /* module display name */
	"ryven",                    /* author's name */
	"2018 ryven.mt@gmail.com",  /* module copyright */
{
	"mdl", NULL, NULL, NULL     /* default extensions to use */
},
_mdl_canload,               /* validation routine */
_mdl_load,                  /* load routine */
NULL,                       /* save validation routine */
NULL                        /* save routine */
};
