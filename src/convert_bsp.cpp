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

   -------------------------------------------------------------------------------

   This code has been altered significantly from its original form, to support
   several games based on the Quake III Arena engine, in the form of "Q3Map2."

   ------------------------------------------------------------------------------- */



/* dependencies */
#include "q3map2.h"

namespace BSPConverter {
	bool isValidSurfaceType(bspSurfaceType_t type) {
		/* ignore patches for now */
		if (!g_onlyModels && type == MST_PLANAR || type == MST_TRIANGLE_SOUP) {
			return true;
		}
		return false;
	}

	bool isValidShaderName(const char *shaderName) {
		if (g_onlyShaders[0][0] != 0) {
			for (int i = 0; i < 10 && g_onlyShaders[i][0]; i++) {
				if (Q_stricmp(g_onlyShaders[i], shaderName) == 0) {
					return true;
				}
			}
			return false;
		}
		return true;
	}
}

/*
   PseudoCompileBSP()
   a stripped down ProcessModels
 */
void PseudoCompileBSP( qboolean need_tree, const char *BSPFilePath, const char *surfaceFilePath ){
	int models;
	char modelValue[10];
	entity_t *entity;
	face_t *faces;
	tree_t *tree;
	node_t *node;
	brush_t *brush;
	side_t *side;
	int i;

	SetDrawSurfacesBuffer();
	mapDrawSurfs = static_cast<mapDrawSurface_t*>(safe_malloc(sizeof( mapDrawSurface_t) * MAX_MAP_DRAW_SURFS));
	memset( mapDrawSurfs, 0, sizeof( mapDrawSurface_t ) * MAX_MAP_DRAW_SURFS );
	numMapDrawSurfs = 0;

	BeginBSPFile();
	models = 1;
	for ( mapEntityNum = 0; mapEntityNum < numEntities; mapEntityNum++ )
	{
		/* get entity */
		entity = &entities[ mapEntityNum ];
		if ( entity->brushes == NULL && entity->patches == NULL ) {
			continue;
		}

		if ( mapEntityNum != 0 ) {
			sprintf( modelValue, "*%d", models++ );
			SetKeyValue( entity, "model", modelValue );
		}

		/* process the model */
		Sys_FPrintf( SYS_VRB, "############### model %i ###############\n", numBSPModels );
		BeginModel();

		entity->firstDrawSurf = numMapDrawSurfs;

		ClearMetaTriangles();
		PatchMapDrawSurfs( entity );

		if ( mapEntityNum == 0 && need_tree ) {
			faces = MakeStructuralBSPFaceList( entities[0].brushes );
			tree = FaceBSP( faces );
			node = tree->headnode;
		}
		else
		{
			node = AllocNode();
			node->planenum = PLANENUM_LEAF;
			tree = AllocTree();
			tree->headnode = node;
		}

		/* cull brush sides */
		CullSides(entity);

		/* a minimized ClipSidesIntoTree */
		for ( brush = entity->brushes; brush; brush = brush->next )
		{
			/* walk the brush sides */
			for ( i = 0; i < brush->numsides; i++ )
			{
				/* get side */
				side = &brush->sides[ i ];
				if ( side->winding == NULL ) {
					continue;
				}
				/* shader? */
				if ( side->shaderInfo == NULL ) {
					continue;
				}
				/* save this winding as a visible surface */
				DrawSurfaceForSide( entity, brush, side, side->winding );
			}
		}

		if ( meta ) {
			ClassifyEntitySurfaces( entity );
			MakeEntityDecals( entity );
			MakeEntityMetaTriangles( entity );
			SmoothMetaTriangles();
			MergeMetaTriangles();
		}
		FilterDrawsurfsIntoTree( entity, tree );

		FilterStructuralBrushesIntoTree( entity, tree );
		FilterDetailBrushesIntoTree( entity, tree );

		EmitBrushes( entity->brushes, &entity->firstBrush, &entity->numBrushes );
		EndModel( entity, node );
	}
	EndBSPFile( qfalse, BSPFilePath, surfaceFilePath );
}

/*
   ConvertBSPMain()
   main argument processing function for bsp conversion
 */

int ConvertBSPMain( int argc, char **argv ){
	int i;
	int ( *convertFunc )( const char *, const char* );
	game_t  *convertGame;
	char ext[1024];
	char BSPFilePath [ 1024 ];
	char surfaceFilePath [ 1024 ];
	qboolean map_allowed, force_bsp, force_map;
	char outputFilePath[1024];

	/* set default */
	convertFunc = ConvertBSPToASE;
	convertGame = NULL;
	map_allowed = qfalse;
	force_bsp = qfalse;
	force_map = qfalse;
	outputFilePath[0] = '\0';

	/* arg checking */
	if ( argc < 1 ) {
		Sys_Printf( "Usage: q3map -convert [-format <ase|obj|map_bp|map>] [-shadersasbitmap|-lightmapsastexcoord|-deluxemapsastexcoord] [-readbsp|-readmap [-meta|-patchmeta]] [-outfile <path>] [-v] <mapname>\n" );
		return 0;
	}

	/* process arguments */
	for ( i = 1; i < ( argc - 1 ); i++ )
	{
		/* -format map|ase|... */
		if (!Q_stricmp(argv[i], "-format")) {
			i++;
			if (!Q_stricmp(argv[i], "ase")) {
				convertFunc = ConvertBSPToASE;
				map_allowed = qtrue;
			}
			else if (!Q_stricmp(argv[i], "obj")) {
				convertFunc = ConvertBSPToOBJ;
				map_allowed = qtrue;
			}
			else if (!Q_stricmp(argv[i], "map_bp")) {
				convertFunc = ConvertBSPToMap_BP;
				map_allowed = qtrue;
			}
			else if (!Q_stricmp(argv[i], "map")) {
				convertFunc = ConvertBSPToMap;
				map_allowed = qtrue;
			}
			else
			{
				convertGame = GetGame(argv[i]);
				map_allowed = qfalse;
				if (convertGame == NULL) {
					Sys_Printf("Unknown conversion format \"%s\". Defaulting to ASE.\n", argv[i]);
				}
			}
		}
		else if (!Q_stricmp(argv[i], "-ne")) {
			normalEpsilon = atof(argv[i + 1]);
			i++;
			Sys_Printf("Normal epsilon set to %f\n", normalEpsilon);
		}
		else if (!Q_stricmp(argv[i], "-de")) {
			distanceEpsilon = atof(argv[i + 1]);
			i++;
			Sys_Printf("Distance epsilon set to %f\n", distanceEpsilon);
		}
		else if (!Q_stricmp(argv[i], "-shaderasbitmap") || !Q_stricmp(argv[i], "-shadersasbitmap")) {
			shadersAsBitmap = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-lightmapastexcoord") || !Q_stricmp(argv[i], "-lightmapsastexcoord")) {
			lightmapsAsTexcoord = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-deluxemapastexcoord") || !Q_stricmp(argv[i], "-deluxemapsastexcoord")) {
			lightmapsAsTexcoord = qtrue;
			deluxemap = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-readbsp")) {
			force_bsp = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-readmap")) {
			force_map = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-meta")) {
			meta = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-patchmeta")) {
			meta = qtrue;
			patchMeta = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-onlymodels")) {
			g_onlyModels = true;
			Sys_Printf("Only models are going to be extracted\n");
		}
		else if (!Q_stricmp(argv[i], "-onlyshaders")) {
			int k = i, finalIndex = argc - 1;
			while (i + 1 < finalIndex && Q_strncasecmp(argv[i + 1], "-", 1)) {
				strncpy(g_onlyShaders[i - k], argv[i + 1], 64);
				i++;
			}
			if (g_onlyShaders[0][0] == 0) {
				Sys_Printf("Warning: -onlyshaders switch expects shader names to be set, but none was found\n");
			}
		}
		else if (!Q_stricmp(argv[i], "-outfile"))
		{
			if (i + 1 < argc - 1)
			{
				i++;
				strcpy(outputFilePath, ExpandArg(argv[i]));
			}
			else
			{
				Sys_Printf("WARNING: -outfile has no path argument\n");
			}
		}
	}

	LoadShaderInfo();

	/* clean up map name */
	strcpy( source, ExpandArg( argv[i] ) );
	ExtractFileExtension( source, ext );

	if ( !map_allowed && !force_map ) {
		force_bsp = qtrue;
	}

	if ( force_map || ( !force_bsp && !Q_stricmp( ext, "map" ) && map_allowed ) ) {
		if ( !map_allowed ) {
			Sys_FPrintf( SYS_WRN, "WARNING: the requested conversion should not be done from .map files. Compile a .bsp first.\n" );
		}
		StripExtension( source );
		DefaultExtension( source, ".map" );
		Sys_Printf( "Loading %s\n", source );
		LoadMapFile( source, qfalse, convertGame == NULL ? qtrue : qfalse );
		sprintf( BSPFilePath, "%s.bsp", source );
		sprintf( surfaceFilePath, "%s.srf", source );
		PseudoCompileBSP( convertGame != NULL ? qtrue : qfalse, BSPFilePath, surfaceFilePath );
	}
	else
	{
		StripExtension( source );
		DefaultExtension( source, ".bsp" );
		Sys_Printf( "Loading %s\n", source );
		LoadBSPFile( source );
		ParseEntities();
	}

	/* bsp format convert? */
	if ( convertGame != NULL ) {
		/* set global game */
		game = convertGame;

		/* write bsp */
		StripExtension( source );
		DefaultExtension( source, "_c.bsp" );
		Sys_Printf( "Writing %s\n", source );
		WriteBSPFile( source );

		/* return to sender */
		return 0;
	}

	/* normal convert */
	return convertFunc( source, outputFilePath );
}
