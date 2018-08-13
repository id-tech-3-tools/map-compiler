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



/* marker */
#define MAIN_C



/* dependencies */
#include "q3map2.h"
#include "table_builder.hpp"

/*
   Random()
   returns a pseudorandom number between 0 and 1
 */

vec_t Random( void ){
	return (vec_t) rand() / RAND_MAX;
}


char *Q_strncpyz( char *dst, const char *src, size_t len ) {
	if ( len == 0 ) {
		abort();
	}

	strncpy( dst, src, len );
	dst[ len - 1 ] = '\0';
	return dst;
}


char *Q_strcat( char *dst, size_t dlen, const char *src ) {
	size_t n = strlen( dst  );

	if ( n > dlen ) {
		abort(); /* buffer overflow */
	}

	return Q_strncpyz( dst + n, src, dlen - n );
}


char *Q_strncat( char *dst, size_t dlen, const char *src, size_t slen ) {
	size_t n = strlen( dst );

	if ( n > dlen ) {
		abort(); /* buffer overflow */
	}

	return Q_strncpyz( dst + n, src, Q_min( slen, dlen - n ) );
}

/*
   ExitQ3Map()
   cleanup routine
 */

static void ExitQ3Map( void ){
	BSPFilesCleanup();
	if ( mapDrawSurfs != NULL ) {
		free( mapDrawSurfs );
	}
}

void printOptions(const std::vector<OptionResult> &options)
{
	TableBuilder table{ 20, 60 };
	for (const auto &option : options)
	{
		if (!option.option.empty())
		{
			table.addRow(option.option + " " + option.value);
		}
		if (!option.desc.empty())
		{
			table.addRow(" " + option.desc);
		}
	}
	auto rows = table.build();
	for (const auto &row : rows)
	{
		Sys_Printf("%s\n", row.c_str());
	}
}

/*
   main()
   q3map mojo...
 */

int main(int argc, char **argv) {
	int i, r;
	double start, end;


	/* we want consistent 'randomness' */
	srand( 0 );

	/* start timer */
	start = I_FloatTime();

	/* this was changed to emit version number over the network */
	// printf( Q3MAP_VERSION "\n" );

	/* set exit call */
	atexit( ExitQ3Map );

	/* set game to default (q3a) */
	game = &games[0];

	/* read general options first */
	for ( i = 1; i < argc; i++ )
	{
		/* -help */
		if (!Q_stricmp(argv[i], "-h") || !Q_stricmp(argv[i], "--help")
			|| !Q_stricmp(argv[i], "-help")) {
			HelpMain(argv[i + 1]);
			return 0;
		}

		/* -connect */
		if (!Q_stricmp(argv[i], "-connect")) {
			i++;
			Broadcast_Setup(argv[i]);
		}

		/* verbose */
		else if (!Q_stricmp(argv[i], "-v")) {
			if (!verbose) {
				verbose = qtrue;
			}
		}

		/* force */
		else if (!Q_stricmp(argv[i], "-force")) {
			force = qtrue;
		}

		/* patch subdivisions */
		else if (!Q_stricmp(argv[i], "-subdivisions")) {
			i++;
			patchSubdivisions = atoi(argv[i]);
			if (patchSubdivisions <= 0) {
				patchSubdivisions = 1;
			}
		}

		/* threads */
		else if (!Q_stricmp(argv[i], "-threads")) {
			i++;
			numthreads = atoi(argv[i]);
		}

		else if (Q_stricmp(argv[i], "-game") == 0) {
			if (++i >= argc) {
				Error("Out of arguments: No game specified after %s", argv[i - 1]);
			}
			game = GetGame(argv[i]);
			if (game == NULL) {
				game = &games[0];
			}
		}
	}

	/* init model library */
	PicoInit();
	PicoSetMallocFunc( safe_malloc );
	PicoSetFreeFunc( free );
	PicoSetPrintFunc( PicoPrintFunc );
	PicoSetLoadFileFunc( PicoLoadFileFunc );
	PicoSetFreeFileFunc( free );

	/* set number of threads */
	ThreadSetDefault();

	/* generate sinusoid jitter table */
	for ( i = 0; i < MAX_JITTERS; i++ )
	{
		jitters[ i ] = sin( i * 139.54152147 );
		//%	Sys_Printf( "Jitter %4d: %f\n", i, jitters[ i ] );
	}

	/* we print out two versions, q3map's main version (since it evolves a bit out of GtkRadiant)
	   and we put the GtkRadiant version to make it easy to track with what version of Radiant it was built with */

	Sys_Printf("\n");
	Sys_Printf("Quake III .map compiler " MAPCOMPILER_VERSION " "  __DATE__ " " __TIME__ "\n");
	Sys_Printf("Patches by Aciz and ryven\n");
	Sys_Printf("https://github.com/isRyven/map-compiler\n");
	Sys_Printf("\n");
	Sys_Printf("Q3Map              - v1.0r (c) 1999 Id Software Inc.  \n");
	Sys_Printf("Q3Map (ydnar)      - v2.5.17                          \n");
	Sys_Printf("Q3Map (NetRadiant) - v2.5.17n                         \n");
	Sys_Printf("%s\n\n", Q3MAP_MOTD);


	/* ydnar: new path initialization */
	InitPaths( argc, argv );

	/* set game options */
	if ( !patchSubdivisions ) {
		patchSubdivisions = game->patchSubdivisions;
	}

	/* check if we have enough options left to attempt something */
	if ( argc < 2 ) {
		Error( "Usage: %s [general options] [options] mapfile", argv[ 0 ] );
	}

	/* fixaas */
	if (!Q_stricmp(argv[1], "-fixaas")) {
		r = FixAASMain(argc - 1, argv + 1);
	}

	/* analyze */
	else if (!Q_stricmp(argv[1], "-analyze")) {
		r = AnalyzeBSPMain(argc - 1, argv + 1);
	}

	/* info */
	else if (!Q_stricmp(argv[1], "-info")) {
		r = BSPInfoMain(argc - 2, argv + 2);
	}

	/* vis */
	else if (!Q_stricmp(argv[1], "-vis")) {
		r = VisMain(argc - 1, argv + 1);
	}

	/* light */
	else if (!Q_stricmp(argv[1], "-light")) {
		r = LightMain(argc - 1, argv + 1);
	}

	/* QBall: export entities */
	else if (!Q_stricmp(argv[1], "-exportents")) {
		r = ExportEntitiesMain(argc - 1, argv + 1);
	}

	/* ydnar: lightmap export */
	else if (!Q_stricmp(argv[1], "-export")) {
		r = ExportLightmapsMain(argc - 1, argv + 1);
	}

	/* ydnar: lightmap import */
	else if (!Q_stricmp(argv[1], "-import")) {
		r = ImportLightmapsMain(argc - 1, argv + 1);
	}

	/* ydnar: bsp scaling */
	else if (!Q_stricmp(argv[1], "-scale")) {
		r = ScaleBSPMain(argc - 1, argv + 1);
	}

	/* ydnar: bsp conversion */
	else if (!Q_stricmp(argv[1], "-convert")) {
		r = ConvertBSPMain(argc - 1, argv + 1);
	}

	/* div0: minimap */
	else if (!Q_stricmp(argv[1], "-minimap")) {
		r = MiniMapBSPMain(argc - 1, argv + 1);
	}

	/* ydnar: otherwise create a bsp */
	else{
		r = BSPMain( argc, argv );
	}

	/* emit time */
	end = I_FloatTime();
	Sys_Printf( "%9.0f seconds elapsed\n", end - start );

	/* shut down connection */
	Broadcast_Shutdown();

	/* return any error code */
	return r;
}
