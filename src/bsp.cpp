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
#define BSP_C

/* dependencies */
#include "q3map2.h"

#undef min
#undef max

#include <vector>
#include <string>
#include "tinyformat.h"

/* -------------------------------------------------------------------------------

   functions

   ------------------------------------------------------------------------------- */

/*
   ProcessAdvertisements()
   copies advertisement info into the BSP structures
 */

static void ProcessAdvertisements( void ) {
	int i;
	const char*         className;
	const char*         modelKey;
	int modelNum;
	bspModel_t*         adModel;
	bspDrawSurface_t*   adSurface;

	Sys_FPrintf( SYS_VRB, "--- ProcessAdvertisements ---\n" );

	for ( i = 0; i < numEntities; i++ ) {

		/* is an advertisement? */
		className = ValueForKey( &entities[ i ], "classname" );

		if ( !Q_stricmp( "advertisement", className ) ) {

			modelKey = ValueForKey( &entities[ i ], "model" );

			if ( strlen( modelKey ) > MAX_QPATH - 1 ) {
				Error( "Model Key for entity exceeds ad struct string length." );
			}
			else {
				if ( numBSPAds < MAX_MAP_ADVERTISEMENTS ) {
					bspAds[numBSPAds].cellId = IntForKey( &entities[ i ], "cellId" );
					strncpy( bspAds[numBSPAds].model, modelKey, sizeof( bspAds[numBSPAds].model ) );

					modelKey++;
					modelNum = atoi( modelKey );
					adModel = &bspModels[modelNum];

					if ( adModel->numBSPSurfaces != 1 ) {
						Error( "Ad cell id %d has more than one surface.", bspAds[numBSPAds].cellId );
					}

					adSurface = &bspDrawSurfaces[adModel->firstBSPSurface];

					// store the normal for use at run time.. all ad verts are assumed to
					// have identical normals (because they should be a simple rectangle)
					// so just use the first vert's normal
					VectorCopy( bspDrawVerts[adSurface->firstVert].normal, bspAds[numBSPAds].normal );

					// store the ad quad for quick use at run time
					if ( adSurface->surfaceType == MST_PATCH ) {
						int v0 = adSurface->firstVert + adSurface->patchHeight - 1;
						int v1 = adSurface->firstVert + adSurface->numVerts - 1;
						int v2 = adSurface->firstVert + adSurface->numVerts - adSurface->patchWidth;
						int v3 = adSurface->firstVert;
						VectorCopy( bspDrawVerts[v0].xyz, bspAds[numBSPAds].rect[0] );
						VectorCopy( bspDrawVerts[v1].xyz, bspAds[numBSPAds].rect[1] );
						VectorCopy( bspDrawVerts[v2].xyz, bspAds[numBSPAds].rect[2] );
						VectorCopy( bspDrawVerts[v3].xyz, bspAds[numBSPAds].rect[3] );
					}
					else {
						Error( "Ad cell %d has an unsupported Ad Surface type.", bspAds[numBSPAds].cellId );
					}

					numBSPAds++;
				}
				else {
					Error( "Maximum number of map advertisements exceeded." );
				}
			}
		}
	}

	Sys_FPrintf( SYS_VRB, "%9d in-game advertisements\n", numBSPAds );
}

/*
   SetCloneModelNumbers() - ydnar
   sets the model numbers for brush entities
 */

static void SetCloneModelNumbers( void ){
	int i, j;
	int models;
	char modelValue[ 10 ];
	const char  *value, *value2, *value3;


	/* start with 1 (worldspawn is model 0) */
	models = 1;
	for ( i = 1; i < numEntities; i++ )
	{
		/* only entities with brushes or patches get a model number */
		if ( entities[ i ].brushes == NULL && entities[ i ].patches == NULL ) {
			continue;
		}

		/* is this a clone? */
		value = ValueForKey( &entities[ i ], "_ins" );
		if ( value[ 0 ] == '\0' ) {
			value = ValueForKey( &entities[ i ], "_instance" );
		}
		if ( value[ 0 ] == '\0' ) {
			value = ValueForKey( &entities[ i ], "_clone" );
		}
		if ( value[ 0 ] != '\0' ) {
			continue;
		}

		/* add the model key */
		sprintf( modelValue, "*%d", models );
		SetKeyValue( &entities[ i ], "model", modelValue );

		/* increment model count */
		models++;
	}

	/* fix up clones */
	for ( i = 1; i < numEntities; i++ )
	{
		/* only entities with brushes or patches get a model number */
		if ( entities[ i ].brushes == NULL && entities[ i ].patches == NULL ) {
			continue;
		}

		/* is this a clone? */
		value = ValueForKey( &entities[ i ], "_ins" );
		if ( value[ 0 ] == '\0' ) {
			value = ValueForKey( &entities[ i ], "_instance" );
		}
		if ( value[ 0 ] == '\0' ) {
			value = ValueForKey( &entities[ i ], "_clone" );
		}
		if ( value[ 0 ] == '\0' ) {
			continue;
		}

		/* find an entity with matching clone name */
		for ( j = 0; j < numEntities; j++ )
		{
			/* is this a clone parent? */
			value2 = ValueForKey( &entities[ j ], "_clonename" );
			if ( value2[ 0 ] == '\0' ) {
				continue;
			}

			/* do they match? */
			if ( strcmp( value, value2 ) == 0 ) {
				/* get the model num */
				value3 = ValueForKey( &entities[ j ], "model" );
				if ( value3[ 0 ] == '\0' ) {
					Sys_FPrintf( SYS_WRN, "WARNING: Cloned entity %s referenced entity without model\n", value2 );
					continue;
				}
				models = atoi( &value2[ 1 ] );

				/* add the model key */
				sprintf( modelValue, "*%d", models );
				SetKeyValue( &entities[ i ], "model", modelValue );

				/* nuke the brushes/patches for this entity (fixme: leak!) */
				entities[ i ].brushes = NULL;
				entities[ i ].patches = NULL;
			}
		}
	}
}



/*
   FixBrushSides() - ydnar
   matches brushsides back to their appropriate drawsurface and shader
 */

static void FixBrushSides( entity_t *e ){
	int i;
	mapDrawSurface_t    *ds;
	sideRef_t           *sideRef;
	bspBrushSide_t      *side;


	/* note it */
	Sys_FPrintf( SYS_VRB, "--- FixBrushSides ---\n" );

	/* walk list of drawsurfaces */
	for ( i = e->firstDrawSurf; i < numMapDrawSurfs; i++ )
	{
		/* get surface and try to early out */
		ds = &mapDrawSurfs[ i ];
		if ( ds->outputNum < 0 ) {
			continue;
		}

		/* walk sideref list */
		for ( sideRef = ds->sideRef; sideRef != NULL; sideRef = sideRef->next )
		{
			/* get bsp brush side */
			if ( sideRef->side == NULL || sideRef->side->outputNum < 0 ) {
				continue;
			}
			side = &bspBrushSides[ sideRef->side->outputNum ];

			/* set drawsurface */
			side->surfaceNum = ds->outputNum;
			//%	Sys_FPrintf( SYS_VRB, "DS: %7d Side: %7d     ", ds->outputNum, sideRef->side->outputNum );

			/* set shader */
			if ( strcmp( bspShaders[ side->shaderNum ].shader, ds->shaderInfo->shader ) ) {
				//%	Sys_FPrintf( SYS_VRB, "Remapping %s to %s\n", bspShaders[ side->shaderNum ].shader, ds->shaderInfo->shader );
				side->shaderNum = EmitShader( ds->shaderInfo->shader, &ds->shaderInfo->contentFlags, &ds->shaderInfo->surfaceFlags );
			}
		}
	}
}



/*
   ProcessWorldModel()
   creates a full bsp + surfaces for the worldspawn entity
 */

void ProcessWorldModel( const char *portalFilePath, const char *lineFilePath ){
	int i, s;
	entity_t    *e;
	tree_t      *tree;
	face_t      *faces;
	qboolean ignoreLeaks, leaked;
	//xmlNodePtr polyline, leaknode;
	char level[ 2 ], shader[ 1024 ];
	const char  *value;
	int leakStatus;

	/* sets integer blockSize from worldspawn "_blocksize" key if it exists */
	value = ValueForKey( &entities[ 0 ], "_blocksize" );
	if ( value[ 0 ] == '\0' ) {
		value = ValueForKey( &entities[ 0 ], "blocksize" );
	}
	if ( value[ 0 ] == '\0' ) {
		value = ValueForKey( &entities[ 0 ], "chopsize" );  /* sof2 */
	}
	if ( value[ 0 ] != '\0' ) {
		/* scan 3 numbers */
		s = sscanf( value, "%d %d %d", &blockSize[ 0 ], &blockSize[ 1 ], &blockSize[ 2 ] );

		/* handle legacy case */
		if ( s == 1 ) {
			blockSize[ 1 ] = blockSize[ 0 ];
			blockSize[ 2 ] = blockSize[ 0 ];
		}
	}
	Sys_Printf( "block size = { %d %d %d }\n", blockSize[ 0 ], blockSize[ 1 ], blockSize[ 2 ] );

	/* sof2: ignore leaks? */
	value = ValueForKey( &entities[ 0 ], "_ignoreleaks" );  /* ydnar */
	if ( value[ 0 ] == '\0' ) {
		value = ValueForKey( &entities[ 0 ], "ignoreleaks" );
	}
	if ( value[ 0 ] == '1' ) {
		ignoreLeaks = qtrue;
	}
	else{
		ignoreLeaks = qfalse;
	}

	/* begin worldspawn model */
	BeginModel();
	e = &entities[ 0 ];
	e->firstDrawSurf = 0;

	/* ydnar: gs mods */
	ClearMetaTriangles();

	/* check for patches with adjacent edges that need to lod together */
	PatchMapDrawSurfs( e );

	/* build an initial bsp tree using all of the sides of all of the structural brushes */
	faces = MakeStructuralBSPFaceList( entities[ 0 ].brushes );
	tree = FaceBSP( faces );
	MakeTreePortals( tree );
	FilterStructuralBrushesIntoTree( e, tree );

	/* see if the bsp is completely enclosed */
	leakStatus = FloodEntities( tree );
	if ( ignoreLeaks ) {
		if ( leakStatus == FLOODENTITIES_LEAKED ) {
			leakStatus = FLOODENTITIES_GOOD;
		}
	}

	if ( leakStatus == FLOODENTITIES_GOOD ) {
		leaked = qfalse;
	}
	else
	{
		leaked = qtrue;

		Sys_FPrintf( SYS_NOXML, "**********************\n" );
		Sys_FPrintf( SYS_NOXML, "******* leaked *******\n" );
		Sys_FPrintf( SYS_NOXML, "**********************\n" );
		LeakFile( tree, lineFilePath );
		//leaknode = xmlNewNode( NULL, (xmlChar*)"message" );
		//xmlNodeSetContent( leaknode, (xmlChar*)"MAP LEAKED\n" );
		//xmlAddChild( leaknode, polyline );
		level[0] = (int) '0' + SYS_ERR;
		level[1] = 0;
		//xmlSetProp( leaknode, (xmlChar*)"level", (xmlChar*) &level );
		//xml_SendNode( leaknode );
		if ( leaktest ) {
			Sys_Printf( "--- MAP LEAKED, ABORTING LEAKTEST ---\n" );
			exit( 0 );
		}

		if (leakStatus == FLOODENTITIES_EMPTY)
		{
			/* chop the sides to the convex hull of their visible fragments, giving us the smallest polygons */
			ClipSidesIntoTree(e, tree);
		}
	}

	if ( leakStatus != FLOODENTITIES_EMPTY ) { /* if no entities exist, this would accidentally the whole map, and that IS bad */
		/* rebuild a better bsp tree using only the sides that are visible from the inside */
		FillOutside( tree->headnode );

		/* chop the sides to the convex hull of their visible fragments, giving us the smallest polygons */
		ClipSidesIntoTree( e, tree );

		/* build a visible face tree (same thing as the initial bsp tree but after reducing the faces) */
		faces = MakeVisibleBSPFaceList( entities[ 0 ].brushes );
		FreeTree( tree );
		tree = FaceBSP( faces );
		MakeTreePortals( tree );
		FilterStructuralBrushesIntoTree( e, tree );

		/* ydnar: flood again for skybox */
		if ( skyboxPresent ) {
			FloodEntities( tree );
		}
	}

	/* save out information for visibility processing */
	NumberClusters( tree );
	if ( !leaked ) {
		WritePortalFile( tree, portalFilePath );
	}

	/* flood from entities */
	FloodAreas( tree );

	/* create drawsurfs for triangle models */
	AddTriangleModels( e );

	/* create drawsurfs for surface models */
	AddEntitySurfaceModels( e );

	/* generate bsp brushes from map brushes */
	EmitBrushes( e->brushes, &e->firstBrush, &e->numBrushes );

	/* add references to the detail brushes */
	FilterDetailBrushesIntoTree( e, tree );

	/* drawsurfs that cross fog boundaries will need to be split along the fog boundary */
	if ( !nofog ) {
		FogDrawSurfaces( e );
	}

	/* subdivide each drawsurf as required by shader tesselation */
	if ( !nosubdivide ) {
		SubdivideFaceSurfaces( e, tree );
	}

	/* add in any vertexes required to fix t-junctions */
	if ( !notjunc ) {
		FixTJunctions( e );
	}

	/* ydnar: classify the surfaces */
	ClassifyEntitySurfaces( e );

	/* ydnar: project decals */
	MakeEntityDecals( e );

	/* ydnar: meta surfaces */
	MakeEntityMetaTriangles( e );
	SmoothMetaTriangles();
	FixMetaTJunctions();
	MergeMetaTriangles();

	/* ydnar: debug portals */
	if ( debugPortals ) {
		MakeDebugPortalSurfs( tree );
	}

	/* ydnar: fog hull */
	value = ValueForKey( &entities[ 0 ], "_foghull" );
	if ( value[ 0 ] != '\0' ) {
		sprintf( shader, "textures/%s", value );
		MakeFogHullSurfs( e, tree, shader );
	}

	/* ydnar: bug 645: do flares for lights */
	for ( i = 0; i < numEntities && emitFlares; i++ )
	{
		entity_t    *light, *target;
		const char  *value, *flareShader;
		vec3_t origin, targetOrigin, normal, color;
		int lightStyle;


		/* get light */
		light = &entities[ i ];
		value = ValueForKey( light, "classname" );
		if ( !strcmp( value, "light" ) ) {
			/* get flare shader */
			flareShader = ValueForKey( light, "_flareshader" );
			value = ValueForKey( light, "_flare" );
			if ( flareShader[ 0 ] != '\0' || value[ 0 ] != '\0' ) {
				/* get specifics */
				GetVectorForKey( light, "origin", origin );
				GetVectorForKey( light, "_color", color );
				lightStyle = IntForKey( light, "_style" );
				if ( lightStyle == 0 ) {
					lightStyle = IntForKey( light, "style" );
				}

				/* handle directional spotlights */
				value = ValueForKey( light, "target" );
				if ( value[ 0 ] != '\0' ) {
					/* get target light */
					target = FindTargetEntity( value );
					if ( target != NULL ) {
						GetVectorForKey( target, "origin", targetOrigin );
						VectorSubtract( targetOrigin, origin, normal );
						VectorNormalize( normal, normal );
					}
				}
				else{
					//%	VectorClear( normal );
					VectorSet( normal, 0, 0, -1 );
				}

				if ( colorsRGB ) {
					color[0] = Image_LinearFloatFromsRGBFloat( color[0] );
					color[1] = Image_LinearFloatFromsRGBFloat( color[1] );
					color[2] = Image_LinearFloatFromsRGBFloat( color[2] );
				}

				/* create the flare surface (note shader defaults automatically) */
				DrawSurfaceForFlare( mapEntityNum, origin, normal, color, flareShader, lightStyle );
			}
		}
	}

	/* add references to the final drawsurfs in the apropriate clusters */
	FilterDrawsurfsIntoTree( e, tree );

	/* match drawsurfaces back to original brushsides (sof2) */
	FixBrushSides( e );

	/* finish */
	EndModel( e, tree->headnode );
	FreeTree( tree );
}



/*
   ProcessSubModel()
   creates bsp + surfaces for other brush models
 */

void ProcessSubModel( void ){
	entity_t    *e;
	tree_t      *tree;
	brush_t     *b, *bc;
	node_t      *node;


	/* start a brush model */
	BeginModel();
	e = &entities[ mapEntityNum ];
	e->firstDrawSurf = numMapDrawSurfs;

	/* ydnar: gs mods */
	ClearMetaTriangles();

	/* check for patches with adjacent edges that need to lod together */
	PatchMapDrawSurfs( e );

	/* allocate a tree */
	node = AllocNode();
	node->planenum = PLANENUM_LEAF;
	tree = AllocTree();
	tree->headnode = node;

	/* add the sides to the tree */
	ClipSidesIntoTree( e, tree );

	/* ydnar: create drawsurfs for triangle models */
	AddTriangleModels( e );

	/* create drawsurfs for surface models */
	AddEntitySurfaceModels( e );

	/* generate bsp brushes from map brushes */
	EmitBrushes( e->brushes, &e->firstBrush, &e->numBrushes );

	/* just put all the brushes in headnode */
	for ( b = e->brushes; b; b = b->next )
	{
		bc = CopyBrush( b );
		bc->next = node->brushlist;
		node->brushlist = bc;
	}

	/* subdivide each drawsurf as required by shader tesselation */
	if ( !nosubdivide ) {
		SubdivideFaceSurfaces( e, tree );
	}

	/* add in any vertexes required to fix t-junctions */
	if ( !notjunc ) {
		FixTJunctions( e );
	}

	/* ydnar: classify the surfaces and project lightmaps */
	ClassifyEntitySurfaces( e );

	/* ydnar: project decals */
	MakeEntityDecals( e );

	/* ydnar: meta surfaces */
	MakeEntityMetaTriangles( e );
	SmoothMetaTriangles();
	FixMetaTJunctions();
	MergeMetaTriangles();

	/* add references to the final drawsurfs in the apropriate clusters */
	FilterDrawsurfsIntoTree( e, tree );

	/* match drawsurfaces back to original brushsides (sof2) */
	FixBrushSides( e );

	/* finish */
	EndModel( e, node );
	FreeTree( tree );
}



/*
   ProcessModels()
   process world + other models into the bsp
 */

void ProcessModels( const char *portalFilePath, const char *lineFilePath ){
	qboolean oldVerbose;
	entity_t    *entity;


	/* preserve -v setting */
	oldVerbose = verbose;

	/* start a new bsp */
	BeginBSPFile();

	/* create map fogs */
	CreateMapFogs();

	/* walk entity list */
	for ( mapEntityNum = 0; mapEntityNum < numEntities; mapEntityNum++ )
	{
		/* get entity */
		entity = &entities[ mapEntityNum ];
		if ( entity->brushes == NULL && entity->patches == NULL ) {
			continue;
		}

		/* process the model */
		Sys_FPrintf( SYS_VRB, "############### model %i ###############\n", numBSPModels );
		if ( mapEntityNum == 0 ) {
			ProcessWorldModel(portalFilePath, lineFilePath);
		}
		else{
			ProcessSubModel();
		}

		/* potentially turn off the deluge of text */
		verbose = verboseEntities;
	}

	/* restore -v setting */
	verbose = oldVerbose;

	/* write fogs */
	EmitFogs();

	/* vortex: emit meta stats */
	EmitMetaStats();
}



/*
   OnlyEnts()
   this is probably broken unless teamed with a radiant version that preserves entity order
 */

void OnlyEnts( const char *BSPFilePath ){
	char save_cmdline[1024], save_version[1024], save_gridsize[1024];
	const char *p;

	/* note it */
	Sys_Printf( "--- OnlyEnts ---\n" );

	LoadBSPFile( BSPFilePath );

	ParseEntities();
	p = ValueForKey( &entities[0], "_q3map2_cmdline" );
	strncpy( save_cmdline, p, sizeof( save_cmdline ) );
	save_cmdline[sizeof( save_cmdline ) - 1] = 0;
	p = ValueForKey( &entities[0], "_q3map2_version" );
	strncpy( save_version, p, sizeof( save_version ) );
	save_version[sizeof( save_version ) - 1] = 0;
	p = ValueForKey( &entities[0], "gridsize" );
	strncpy( save_gridsize, p, sizeof( save_gridsize ) );
	save_gridsize[sizeof( save_gridsize ) - 1] = 0;

	numEntities = 0;

	LoadShaderInfo();
	LoadMapFile( name, qfalse, qfalse );
	SetModelNumbers();
	SetLightStyles();

	if ( *save_cmdline ) {
		SetKeyValue( &entities[0], "_q3map2_cmdline", save_cmdline );
	}
	if ( *save_version ) {
		SetKeyValue( &entities[0], "_q3map2_version", save_version );
	}
	if ( *save_gridsize ) {
		SetKeyValue( &entities[0], "gridsize", save_gridsize );
	}

	numBSPEntities = numEntities;
	UnparseEntities();

	WriteBSPFile( BSPFilePath );
}



/*
   BSPMain() - ydnar
   handles creation of a bsp from a map file
 */

int BSPMain( int argc, char **argv ){
	int i;
	char path[ 1024 ], tempSource[ 1024 ];
	qboolean onlyents = qfalse;
	char BSPFilePath [ 1024 ];
	char lineFilePath [ 1024 ];
	char portalFilePath [ 1024 ];
	char surfaceFilePath [ 1024 ];
	BSPFilePath[0] = 0;
	lineFilePath[0] = 0;
	portalFilePath[0] = 0;
	surfaceFilePath[0] = 0;

	std::vector<OptionResult> options;
	options.push_back({ "-bsp" });

	/* note it */
	Sys_Printf( "--- BSP ---\n" );

	SetDrawSurfacesBuffer();
	mapDrawSurfs = static_cast<mapDrawSurface_t*>(safe_malloc(sizeof( mapDrawSurface_t) * MAX_MAP_DRAW_SURFS));
	memset( mapDrawSurfs, 0, sizeof( mapDrawSurface_t ) * MAX_MAP_DRAW_SURFS );
	numMapDrawSurfs = 0;

	tempSource[ 0 ] = '\0';
	globalCelShader[0] = 0;

	/* set standard game flags */
	maxSurfaceVerts = game->maxSurfaceVerts;
	maxSurfaceIndexes = game->maxSurfaceIndexes;
	emitFlares = game->emitFlares;
	texturesRGB = game->texturesRGB;
	colorsRGB = game->colorsRGB;

	/* process arguments */
	for ( i = 1; i < ( argc - 1 ); i++ )
	{
		if (!Q_stricmp(argv[i], "-onlyents")) {
			options.push_back({ argv[i], "", "running entity-only compile" });
			onlyents = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-tempname")) {
			strcpy(tempSource, argv[++i]);
		}
		else if (!Q_stricmp(argv[i], "-tmpout")) {
			strcpy(outbase, "/tmp");
		}
		else if (!Q_stricmp(argv[i], "-nowater")) {
			options.push_back({ argv[i], "", "disabling water" });
			nowater = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-keeplights")) {
			options.push_back({ argv[i], "", "leaving light entities on map after compile" });
			keepLights = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-nodetail")) {
			options.push_back({ argv[i], "", "ignoring detail brushes" });
			nodetail = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-fulldetail")) {
			options.push_back({ argv[i], "", "turning detail brushes into structural brushes" });
			fulldetail = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-nofog")) {
			options.push_back({ argv[i], "", "fog volumes disabled" });
			nofog = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-nosubdivide")) {
			options.push_back({ argv[i], "", "disabling brush face subdivision" });
			nosubdivide = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-leaktest")) {
			options.push_back({ argv[i], "", "leaktest enabled" });
			leaktest = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-verboseentities")) {
			options.push_back({ argv[i], "", "verbose entities enabled" });
			verboseEntities = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-nocurves")) {
			options.push_back({ argv[i], "", "ignoring curved surfaces (patches)" });
			noCurveBrushes = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-notjunc")) {
			options.push_back({ argv[i], "", "t-junction fixing disabled" });
			notjunc = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-fakemap")) {
			options.push_back({ argv[i], "", "generating fakemap.map" });
			fakemap = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-samplesize")) {
			sampleSize = std::max(atoi(argv[i + 1]), 1);
			options.push_back({ 
				argv[i], argv[i + 1], 
				tfm::format("lightmap sample size set to %dx%d units", sampleSize, sampleSize) 
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-minsamplesize")) {
			minSampleSize = std::max(atoi(argv[i + 1]), 1);
			options.push_back({
				argv[i], argv[i + 1],
				tfm::format("minimum lightmap sample size set to %dx%d units", minSampleSize, minSampleSize)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-custinfoparms")) {
			options.push_back({ argv[i], "", "custom info parms enabled" });
			useCustomInfoParms = qtrue;
		}

		/* sof2 args */
		else if (!Q_stricmp(argv[i], "-rename")) {
			options.push_back({ argv[i], "", "appending _bsp suffix to misc_model shaders (SOF2)" });
			renameModelShaders = qtrue;
		}

		/* ydnar args */
		else if (!Q_stricmp(argv[i], "-ne")) {
			normalEpsilon = atof(argv[i + 1]);
			options.push_back({ argv[i], argv[i + 1], tfm::format("normal epsilon set to %f", normalEpsilon) });
			i++;
		}
		else if (!Q_stricmp(argv[i], "-de")) {
			distanceEpsilon = atof(argv[i + 1]);
			options.push_back({ argv[i], argv[i + 1], tfm::format("distance epsilon set to %f", distanceEpsilon) });
			i++;
		}
		else if (!Q_stricmp(argv[i], "-mv")) {
			maxLMSurfaceVerts = std::max(atoi(argv[i + 1]), 3);
			maxSurfaceVerts = std::max(maxLMSurfaceVerts, maxSurfaceVerts);
			options.push_back({ 
				argv[i], argv[i + 1], 
				tfm::format("maximum lightmapped surface vertex count set to %d", maxLMSurfaceVerts)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-mi")) {
			maxSurfaceIndexes = std::max(atoi(argv[i + 1]), 3);
			options.push_back({
				argv[i], argv[i + 1],
				tfm::format("maximum per-surface index count set to %d", maxSurfaceIndexes)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-np")) {
			npDegrees = std::max(atof(argv[i + 1]), 0.0);
			options.push_back({
				argv[i], argv[i + 1],
				tfm::format("forcing nonplanar surfaces with a breaking angle of %f degrees", npDegrees)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-snap")) {
			bevelSnap = std::max(atoi(argv[i + 1]), 0);
			options.push_back({
				argv[i], argv[i + 1], tfm::format("snapping brush bevel planes to %d units", bevelSnap)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-texrange")) {
			texRange = std::max(atoi(argv[i + 1]), 0);
			options.push_back({
				argv[i], argv[i + 1], tfm::format("limiting per-surface texture range to %d texels", texRange)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-nohint")) {
			options.push_back({ argv[i], "", "hint brushes disabled" });
			noHint = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-flat")) {
			options.push_back({ argv[i], "", "flatshading enabled" });
			flat = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-celshader")) {
			sprintf(globalCelShader, "textures/%s", argv[i + 1]);
			options.push_back({ 
				argv[i], argv[i + 1], tfm::format("global cel shader set to \"%s\"", globalCelShader)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-meta")) {
			options.push_back({ argv[i], "", "creating meta surfaces from brush faces" });
			meta = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-metaadequatescore")) {
			metaAdequateScore = atoi(argv[i + 1]);
			if (metaAdequateScore < 0) {
				metaAdequateScore = -1;
			}
			options.push_back({ 
				argv[i], argv[i + 1], 
				tfm::format("setting ADEQUATE meta score to %d (see surface_meta.c)", metaAdequateScore)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-metagoodscore")) {
			metaGoodScore = atoi(argv[i + 1]);
			if (metaGoodScore < 0) {
				metaGoodScore = -1;
			}
			options.push_back({
				argv[i], argv[i + 1],
				tfm::format("setting GOOD meta score to %d (see surface_meta.c)", metaGoodScore)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-metamaxbboxdistance")) {
			metaMaxBBoxDistance = atof(argv[i + 1]);
			if (metaMaxBBoxDistance < 0) {
				metaMaxBBoxDistance = -1;
			}
			options.push_back({
				argv[i], argv[i + 1],
				tfm::format("setting meta maximum bounding box distance to %f", metaMaxBBoxDistance)
			});
			i++;
		}
		else if (!Q_stricmp(argv[i], "-patchmeta")) {
			options.push_back({ argv[i], "", "creating meta surfaces from patches" });
			patchMeta = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-flares")) {
			options.push_back({ argv[i], "", "flare surfaces enabled" });
			emitFlares = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-noflares")) {
			options.push_back({ argv[i], "", "flare surfaces disabled" });
			emitFlares = qfalse;
		}
		else if (!Q_stricmp(argv[i], "-skyfix")) {
			options.push_back({ argv[i], "", "GL_CLAMP sky fix/hack/workaround enabled" });
			skyFixHack = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-debugsurfaces")) {
			options.push_back({ argv[i], "", "emitting debug surfaces" });
			debugSurfaces = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-debuginset")) {
			options.push_back({ argv[i], "", "debug surface triangle insetting enabled" });
			debugInset = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-debugportals")) {
			options.push_back({ argv[i], "", "debug portal surfaces enabled" });
			debugPortals = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-sRGBtex")) {
			options.push_back({ argv[i], "", "textures are in sRGB" });
			texturesRGB = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-nosRGBtex")) {
			options.push_back({ argv[i], "", "textures are linear" });
			texturesRGB = qfalse;
		}
		else if (!Q_stricmp(argv[i], "-sRGBcolor")) {
			options.push_back({ argv[i], "", "colors are in sRGB" });
			colorsRGB = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-nosRGBcolor")) {
			options.push_back({ argv[i], "", "colors are linear" });
			colorsRGB = qfalse;
		}
		else if (!Q_stricmp(argv[i], "-nosRGB")) {
			options.push_back({ argv[i], "", "textures are linear, colors are linear" });
			texturesRGB = qfalse;
			colorsRGB = qfalse;
		}
		else if (!Q_stricmp(argv[i], "-altsplit")) {
			options.push_back({ argv[i], "", "alternate BSP splitting enabled" });
			bspAlternateSplitWeights = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-deep")) {
			options.push_back({ argv[i], "", "deep BSP tree generation enabled" });
			deepBSP = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-maxarea")) {
			options.push_back({ argv[i], "", "max Area face surface generation enabled" });
			maxAreaFaceSurface = qtrue;
		}
		else if (!Q_stricmp(argv[i], "-bspfile"))
		{
			strcpy(BSPFilePath, argv[i + 1]);
			options.push_back({ argv[i], argv[i + 1], tfm::format("use %s as bsp file", BSPFilePath) });
			i++;
		}
		else if (!Q_stricmp(argv[i], "-linfile"))
		{
			strcpy(lineFilePath, argv[i + 1]);
			options.push_back({ argv[i], argv[i + 1], tfm::format("use %s as line file", lineFilePath) });
			i++;
		}
		else if (!Q_stricmp(argv[i], "-prtfile"))
		{
			strcpy(portalFilePath, argv[i + 1]);
			options.push_back({ argv[i], argv[i + 1], tfm::format("use %s as portal file", portalFilePath) });
			i++;
		}
		else if (!Q_stricmp(argv[i], "-srffile"))
		{
			strcpy(surfaceFilePath, argv[i + 1]);
			options.push_back({ argv[i], argv[i + 1], tfm::format("use %s as surface file", surfaceFilePath) });
			i++;
		}
		else if (!Q_stricmp(argv[i], "-automapcoords"))
		{
			options.push_back({ argv[i], "", "map coords will be set automatically" });
			g_autoMapCoords = true;
		}
		else if (!Q_stricmp(argv[i], "-automapcoordspad"))
		{
			g_autoMapCoordsPad = atof(argv[i + 1]);
			options.push_back({
				argv[i], argv[i + 1],
				tfm::format("map coords padding is set to %.3f%%\n", g_autoMapCoordsPad * 100.f)
			});
			i++;
		}
	}

	Sys_Printf("BSP Options:\n");
	printOptions(options);
	Sys_Printf("--------------------------\n");

	/* copy source name */
	strcpy( source, ExpandArg( argv[ i ] ) );
	StripExtension( source );

	/* ydnar: set default sample size */
	SetDefaultSampleSize( sampleSize );

	if (!BSPFilePath[0]) {
		sprintf( BSPFilePath, "%s.bsp", source );
	}
	if (!lineFilePath[0]) {
		sprintf( lineFilePath, "%s.lin", source );
	}
	if (!portalFilePath[0]) {
		sprintf( portalFilePath, "%s.prt", source );
	}
	if (!surfaceFilePath[0]) {
		sprintf( surfaceFilePath, "%s.srf", source );
	}

	/* delete portal, line and surface files */
	remove( portalFilePath );
	remove( lineFilePath );
	//%	remove( surfaceFilePath )	/* ydnar */

	/* expand mapname */
	strcpy( name, ExpandArg( argv[ i ] ) );
	if ( strcmp( name + strlen( name ) - 4, ".reg" ) ) {
		/* if we are doing a full map, delete the last saved region map */
		sprintf( path, "%s.reg", source );
		remove( path );
		DefaultExtension( name, ".map" );   /* might be .reg */
	}

	/* if onlyents, just grab the entites and resave */
	if ( onlyents ) {
		OnlyEnts( BSPFilePath );
		return 0;
	}

	/* load shaders */
	LoadShaderInfo();

	/* load original file from temp spot in case it was renamed by the editor on the way in */
	if ( strlen( tempSource ) > 0 ) {
		LoadMapFile( tempSource, qfalse, qfalse );
	}
	else{
		LoadMapFile( name, qfalse, qfalse );
	}

	/* div0: inject command line parameters */
	InjectCommandLine(options);

	/* ydnar: decal setup */
	ProcessDecals();

	/* ydnar: cloned brush model entities */
	SetCloneModelNumbers();

	/* process world and submodels */
	ProcessModels( portalFilePath, lineFilePath );

	/* set light styles from targetted light entities */
	SetLightStyles();

	/* process in game advertisements */
	ProcessAdvertisements();

	/* finish and write bsp */
	EndBSPFile( qtrue, BSPFilePath, surfaceFilePath );

	/* remove temp map source file if appropriate */
	if ( strlen( tempSource ) > 0 ) {
		remove( tempSource );
	}

	/* return to sender */
	return 0;
}
