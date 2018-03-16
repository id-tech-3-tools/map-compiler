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



struct HelpOption
{
	const char* name;
	const char* description;
};

void HelpOptions(const char* group_name, int indentation, int width, struct HelpOption* options, int count)
{
	indentation *= 2;
	char* indent = static_cast<char*>(malloc(indentation + 1));
	memset(indent, ' ', indentation);
	indent[indentation] = 0;
	printf("%s%s:\n", indent, group_name);
	indentation += 2;
	indent = static_cast<char*>(realloc(indent, indentation + 1));
	memset(indent, ' ', indentation);
	indent[indentation] = 0;

	int i;
	for ( i = 0; i < count; i++ )
	{
		int printed = printf("%s%-36s  ", indent, options[i].name);
		int descsz = strlen(options[i].description);
		int j = 0;
		while ( j < descsz && descsz-j > width - printed )
		{
			if ( j != 0 )
				printf("%s%38c",indent,' ');
			int fragment = width - printed;
			while ( fragment > 0 && options[i].description[j+fragment-1] != ' ')
					fragment--;
			j += fwrite(options[i].description+j, sizeof(char), fragment, stdout);
			putchar('\n');
			printed = indentation+38;
		}
		if ( j == 0 )
		{
			printf("%s\n",options[i].description+j);
		}
		else if ( j < descsz )
		{
			printf("%s%38c%s\n",indent,' ',options[i].description+j);
		}
	}

	putchar('\n');

	free(indent);
}

void HelpBsp()
{
	struct HelpOption bsp[] = {
		{"-bsp <filename.map>", "Switch that enters this stage"},
		{"-altsplit", "Alternate BSP tree splitting weights (should give more fps)"},
		{"-automapcoords", "Automatically write mapcoords to worldspawn using map boundaries"},
		{"-automapcoordspad <F>", "Padding applied to sides of autogenerated mapcoords (normalized percentage value)"},
		{"-bspfile <filename.bsp>", "BSP file to write"},
		{"-celshader <shadername>", "Sets a global cel shader name"},
		{"-custinfoparms", "Read scripts/custinfoparms.txt"},
		{"-de <F>", "Sets distance epsilon for plane snapping"},
		{"-debuginset", "Push all triangle vertexes towards the triangle center"},
		{"-debugportals", "Make BSP portals visible in the map"},
		{"-debugsurfaces", "Color the vertexes according to the index of the surface"},
		{"-deep", "Use detail brushes in the BSP tree, but at lowest priority (should give more fps)"},
		{"-fakemap", "Write fakemap.map containing all world brushes"},
		{"-flares", "Turn on support for flares"},
		{"-flat", "Enable flat shading (good for combining with -celshader)"},
		{"-fulldetail", "Treat detail brushes as structural ones"},
		{"-keeplights", "Keep light entities in the BSP file after compile"},
		{"-leaktest", "Abort if a leak was found"},
		{"-linfile <filename.lin>", "Lin file to write"},
		{"-maxarea", "Alternative, more GPU friendly face splitting"},
		{"-meta", "Combine adjacent triangles of the same texture to surfaces (ALWAYS USE THIS)"},
		{"-metaadequatescore <N>", "Sets adequate meta surface score"},
		{"-metagoodscore <N>", "Sets good meta surface score"},
		{"-metamaxbboxdistance <F>", "Sets maximum meta bounding box distance"},
		{"-mi <N>", "Sets the maximum number of indexes per surface"},
		{"-minsamplesize <N>", "Sets minimum lightmap resolution in units/px"},
		{"-mv <N>", "Sets the maximum number of vertices of a lightmapped surface"},
		{"-ne <F>", "Normal epsilon for plane snapping"},
		{"-nocurves", "Turn off support for patches"},
		{"-nodetail", "Leave out detail brushes"},
		{"-noflares", "Turn off support for flares"},
		{"-nofog", "Turn off support for fog volumes"},
		{"-nohint", "Turn off support for hint brushes"},
		{"-nosRGBcolor", "Disable sRGB mode for flares"},
		{"-nosubdivide", "Turn off support for `q3map_tessSize`"},
		{"-notjunc", "Do not fix T-junctions"},
		{"-nowater", "Turn off support for water, slime or lava"},
		{"-np <F>", "Force all surfaces to be nonplanar with a given shade angle"},
		{"-onlyents", "Only update entities in the BSP, point compilation to .ent instead of .map"},
		{"-patchmeta", "Creates meta surfaces from patch meshes"},
		{"-prtfile <filename.prt>", "Portal file to write"},
		{"-rename", "Append suffix to miscmodel shaders (needed for SoF2)"},
		{"-samplesize <N>", "Sets default lightmap resolution in units/px"},
		{"-skyfix", "Turn sky box into six surfaces, redundant on modern hardware"},
		{"-snap <N>", "Snap brush bevel planes to the given number of units"},
		{"-sRGBcolor", "Enable sRGB mode for flares"},
		{"-srffile <filename.srf>", "Surface file to write"},
		{"-tempname <filename.map>", "Read the MAP file from the given file name"},
		{"-texrange <N>", "Limit per-surface texture range to the given number of units, and subdivide surfaces like with `q3map_tessSize` if this is not met"},
		{"-verboseentities", "Enable `-v` only for map entities, not for the world"},
	};
	HelpOptions("BSP Stage", 0, 100, bsp, sizeof(bsp)/sizeof(struct HelpOption));
}

void HelpVis()
{
	struct HelpOption vis[] = {
		{"-vis <filename.map>", "Switch that enters this stage"},
		{"-fast", "Very fast and crude vis calculation"},
		{"-hint", "Faster but still decent vis calculation"},
		{"-merge", "Faster but still okay vis calculation"},
		{"-mergeportals", "The less crude half of `-merge`, makes vis sometimes much faster but doesn't hurt fps usually"},
		{"-nopassage", "Just use PortalFlow vis (usually less fps)"},
		{"-nosort", "Do not sort the portals before calculating vis (usually slower)"},
		{"-passageOnly", "Just use PassageFlow vis (usually less fps)"},
		{"-prtfile <filename.prt>", "Portal file to read"},
		{"-saveprt", "Keep the Portal file after running vis (so you can run vis again)"},
	};
	HelpOptions("VIS Stage", 0, 100, vis, sizeof(vis)/sizeof(struct HelpOption));
}

void HelpLight()
{
	struct HelpOption light[] = {
		{"-light <filename.map>", "Switch that enters this stage"},
		{"-approx <N>", "Vertex light approximation tolerance (never use in conjunction with deluxemapping)"},
		{"-area <F>, -areascale <F>", "Scaling factor for area lights (surfacelight)"},
		{"-border", "Add a red border to lightmaps for debugging"},
		{"-bounce <N>", "Number of bounces for radiosity"},
		{"-bouncegrid", "Also compute radiosity on the light grid"},
		{"-bounceonly", "Only compute radiosity"},
		{"-bouncescale <F>", "Scaling factor for radiosity"},
		{"-bspfile <filename.bsp>", "BSP file to read and write"},
		{"-cheap", "Abort vertex light calculations when white is reached"},
		{"-cheapgrid", "Use `-cheap` style lighting for lightgrid"},
		{"-compensate <F>", "Lightmap compensate (darkening factor applied after everything else)"},
		{"-cpma", "CPMA vertex lighting mode"},
		{"-dark", "Darken lightmap seams"},
		{"-debug", "Mark the lightmaps according to the cluster: unmapped clusters get yellow, occluded ones get pink, flooded ones get blue overlay color, otherwise red"},
		{"-debugaxis", "Color the lightmaps according to the lightmap axis"},
		{"-debugcluster", "Color the lightmaps according to the index of the cluster"},
		{"-debugdeluxe", "Show deluxemaps on the lightmap"},
		{"-debugnormals", "Color the lightmaps according to the direction of the surface normal"},
		{"-debugorigin", "Color the lightmaps according to the origin of the luxels"},
		{"-debugsurface, -debugsurfaces", "Color the lightmaps according to the index of the surface"},
		{"-deluxemode <N>", "0 = modelspace deluxemaps (default), 1 = tangentspace deluxemaps"},
		{"-deluxe, -deluxemap", "Enable deluxemapping (light direction maps)"},
		{"-dirtdebug, -debugdirt", "Store the dirtmaps as lightmaps for debugging"},
		{"-dirtdepth <F>", "Dirtmapping depth"},
		{"-dirtgain <F>", "Dirtmapping exponent"},
		{"-dirtmode <N>", "0 = Ordered direction dirtmapping (default), 1 = randomized direction dirtmapping"},
		{"-dirtscale <F>", "Dirtmapping scaling factor"},
		{"-dirty", "Enable dirtmapping"},
		{"-dump", "Dump radiosity from `-bounce` into numbered MAP file prefabs"},
		{"-export", "Export lightmaps when compile finished (like `-export` mode)"},
		{"-exposure <F>", "Lightmap exposure to better support overbright spots"},
		{"-external", "Force external lightmaps even if at size of internal lightmaps"},
		{"-extradist <F>", "Extra radius for entity lights to decrease overall contribution"},
		{"-extravisnudge", "Nudge luxel origins to a better VIS cluster"},
		{"-fast", "Ignore tiny light contributions"},
		{"-fastbounce", "Use `-fast` style lighting for radiosity"},
		{"-faster", "Use a faster falloff curve for lighting; also implies `-fast`"},
		{"-fastgrid", "Use `-fast` style lighting for the lightgrid"},
		{"-fill", "Fill lightmaps with surrounding colors to improve JPG compression"},
		{"-filter", "Lightmap filtering"},
		{"-floodlight", "Enable floodlight (zero-effort somewhat decent lighting)"},
		{"-gamma <F>", "Lightmap gamma"},
		{"-gridambientdirectionality <F>", "Trade off ambient light in favor of directional light in lightgrid"},
		{"-gridambientscale <F>", "Scaling factor for the light grid ambient components only"},
		{"-griddirectionality <F>", "Trade off directional light in favor of ambient light in lightgrid"},
		{"-gridscale <F>", "Scaling factor for the light grid only"},
		{"-lightanglehl", "Enable Half Lambert lighting attenuation"},
		{"-lightmapdir <path>", "Directory to store external lightmaps (default: same as map name without extension)"},
		{"-lightmapsearchblocksize <N>", "Sets of lightmap search blocksize"},
		{"-lightmapsearchpower <N>", "Sets of lightmap search power"},
		{"-lightmapsize <N>", "Size of lightmaps to generate (must be a power of two)"},
		{"-lightsubdiv <N>", "Size of light emitting shader subdivision"},
		{"-lomem", "Low memory but slower lighting mode"},
		{"-lowquality", "Low quality floodlight (currently breaks floodlight, do not use)"},
		{"-minsamplesize <N>", "Sets minimum lightmap resolution in units/px"},
		{"-nocollapse", "Do not collapse identical lightmaps"},
		{"-nofastpoint", "Disable automatic fast mode for point lights"},
		{"-nofloodstyles", "Disable floodlighting on styled lightmaps"},
		{"-nodeluxe, -nodeluxemap", "Disable deluxemapping"},
		{"-nogrid", "Disable grid light calculation (makes all entities fullbright)"},
		{"-nolightmapsearch", "Do not optimize lightmap packing for GPU memory usage (as doing so costs fps)"},
		{"-nosRGB", "Disable sRGB color for lightmaps, textures and light colors"},
		{"-nosRGBcolor", "Disable sRGB color for light colors"},
		{"-nosRGBlight", "Disable sRGB color for lightmaps"},
		{"-nosRGBtex", "Disable sRGB color for textures (for sampling radiosity)"},
		{"-normalmap", "Color the lightmaps according to the direction of the surface normal (same as `-debugnormals`)"},
		{"-nostyle, -nostyles", "Disable support for light styles"},
		{"-nosurf", "Disable tracing against surfaces (only uses BSP nodes then)"},
		{"-notrace", "Disable shadow occlusion"},
		{"-patchshadows", "Cast shadows from patches"},
		{"-point <F>, -pointscale <F>", "Scaling factor for point lights (light entities)"},
		{"-q3", "Use nonlinear falloff curve by default (like Q3A)"},
		{"-randomsamples", "Use random luxels selection with `-samples`"},
		{"-rawlightmapsizelimit <N>", "Limits raw lightmap size"},
		{"-samples <N>", "Adaptive supersampling quality"},
		{"-samplescale <N>", "Scales all lightmap resolutions"},
		{"-samplesize <N>", "Sets default lightmap resolution in units/px"},
		{"-samplessearchboxsize  <N>", "Multiplies search box size of `-samples`"},
		{"-scale <F>", "Scaling factor for all light types"},
		{"-shade", "Enable phong shading at default shade angle"},
		{"-shadeangle <F>", "Enable phong shading with specified angle"},
		{"-sky <F>, -skyscale <F>", "Scaling factor for sky and sun light"},
		{"-spherical <F>, -sphericalscale <F>", "Scaling factor for spherical point lights"},
		{"-spot <F>, -spotscale <F>", "Scaling factor for spot point lights"},
		{"-srffile <filename.srf>", "Surface file to read"},
		{"-sRGB", "Enable sRGB color for lightmaps, textures and light colors"},
		{"-sRGBcolor", "Enable sRGB color for light colors"},
		{"-sRGBlight", "Enable sRGB color for lightmaps"},
		{"-sRGBtex", "Enable sRGB color for textures (for sampling radiosity)"},
		{"-style, -styles", "Enable support for light styles"},
		{"-sunonly", "Only compute sun light"},
		{"-super <N>, -supersample <N>", "Ordered grid supersampling quality"},
		{"-thresh <F>", "Triangle subdivision threshold"},
		{"-trianglecheck", "Nudges luxels to their original triangle"},
		{"-trisoup", "Convert brush faces to triangle soup"},
		{"-wolf", "Use linear falloff curve by default (like W:ET)"},
	};

	HelpOptions("Light Stage", 0, 100, light, sizeof(light)/sizeof(struct HelpOption));
}

void HelpAnalyze()
{
	struct HelpOption analyze[] = {
		{"-analyze <filename.bsp>", "Switch that enters this mode"},
		{"-lumpswap", "Swap byte order in the lumps"},
	};

	HelpOptions("Analyzing BSP-like file structure", 0, 100, analyze, sizeof(analyze)/sizeof(struct HelpOption));
}

void HelpScale()
{
	struct HelpOption scale[] = {
		{"-scale <S filename.bsp>", "Scale uniformly"},
		{"-scale <SX SY SZ filename.bsp>", "Scale non-uniformly"},
		{"-scale -tex <S filename.bsp>", "Scale uniformly without texture lock"},
		{"-scale -tex <SX SY SZ filename.bsp>", "Scale non-uniformly without texture lock"},
	};
	HelpOptions("Scaling", 0, 100, scale, sizeof(scale)/sizeof(struct HelpOption));
}

void HelpConvert()
{
	struct HelpOption convert[] = {
		{"-convert <filename.bsp>", "Switch that enters this mode"},
		{"-de <F>", "Distance epsilon for the conversion"},
		{"-format <converter>", "Select the converter (available: map, map_bp, ase, obj or game names)"},
		{"-ne <F>", "Normal epsilon for the conversion"},
		{"-shadersasbitmap", "(only for ase) use the shader names as \\*BITMAP key so they work as prefabs"},
		{"-lightmapastexcoord", "Write lightmaps as materials for UV mapping"},
		{"-deluxemapastexcoord", "Write deluxemaps as materials for UV mapping"},
		{"-outfile <path>", "Output path for conversion"},
		{"-readbsp", "Force BSP reading even when not supported by conversion"},
		{"-readmap", "Force MAP reading even when not supported by conversion"},
		{"-meta", "Create meta surfaces during conversion (for models)"},
		{"-patchmeta", "Create meta surfaces from patches during conversion (for models)"},
	};

	HelpOptions("Converting & Decompiling", 0, 100, convert, sizeof(convert)/sizeof(struct HelpOption));
}

void HelpExport()
{
	struct HelpOption exportl[] = {
		{"-export <filename.bsp>", "Copies lightmaps from the BSP to `filename/lightmap_0000.tga`"}
	};

	HelpOptions("Exporting lightmaps", 0, 100, exportl, sizeof(exportl)/sizeof(struct HelpOption));
}

void HelpExportEnts()
{
	struct HelpOption exportents[] = {
		{"-exportents <filename.bsp>", "Exports the entities to a text file (.ent)"},
	};
	HelpOptions("ExportEnts Stage", 0, 100, exportents, sizeof(exportents)/sizeof(struct HelpOption));
}

void HelpFixaas()
{
	struct HelpOption fixaas[] = {
		{"-fixaas <filename.bsp>", "Switch that enters this mode"},
	};

	HelpOptions("Fixing AAS checksum", 0, 100, fixaas, sizeof(fixaas)/sizeof(struct HelpOption));
}

void HelpInfo()
{
	struct HelpOption info[] = {
		{"-info <filename.bsp>", "Switch that enters this mode"},
	};

	HelpOptions("Get info about BSP file", 0, 100, info, sizeof(info)/sizeof(struct HelpOption));
}

void HelpImport()
{
	struct HelpOption import[] = {
		{"-import <filename.bsp>", "Copies lightmaps from `filename/lightmap_0000.tga` into the BSP"},
	};

	HelpOptions("Importing lightmaps", 0, 100, import, sizeof(import)/sizeof(struct HelpOption));
}

void HelpMinimap()
{
	struct HelpOption minimap[] = {
		{"-minimap <filename.bsp>", "Creates a minimap of the BSP, by default writes to `mapname.tga`"},
		{"-autolevel", "Enable automatic height based brightness/contrast adjustment"},
		{"-black", "Write the minimap as a black-on-transparency RGBA32 image"},
		{"-boost <F>", "Sets the contrast boost value (higher values make a brighter image); contrast boost is somewhat similar to gamma, but continuous even at zero"},
		{"-border <F>", "Sets the amount of border pixels relative to the total image size"},
		{"-gray", "Write the minimap as a white-on-black GRAY8 image"},
		{"-keepaspect", "Ensure the aspect ratio is kept (the minimap is then letterboxed to keep aspect)"},
		{"-minmax <xmin ymin zmin xmax ymax zmax>", "Forces specific map dimensions (note: the minimap actually uses these dimensions, scaled to the target size while keeping aspect with centering, and 1/64 of border appended to all sides)"},
		{"-noautolevel", "Disable automatic height based brightness/contrast adjustment"},
		{"-nokeepaspect", "Do not ensure the aspect ratio is kept (makes it easier to use the image in your code, but looks bad together with sharpening)"},
		{"-o <filename.tga>", "Sets the output file name"},
		{"-random <N>", "Sets the randomized supersampling count (cannot be combined with `-samples`)"},
		{"-samples <N>", "Sets the ordered supersampling count (cannot be combined with `-random`)"},
		{"-sharpen <F>", "Sets the sharpening coefficient"},
		{"-size <N>", "Sets the width and height of the output image"},
		{"-white", "Write the minimap as a white-on-transparency RGBA32 image"},
	};

	HelpOptions("MiniMap", 0, 100, minimap, sizeof(minimap)/sizeof(struct HelpOption));
}

void HelpCommon()
{
	struct HelpOption common[] = {
		{"-force", "Allow reading some broken/unsupported BSP files e.g. when decompiling, may also crash"},
		{"-fs_basepath <path>", "Sets the given path to read assets from (up to 10)"},
		{"-fs_forbiddenpath <path>", "Stops reading assets from given path (up to 64)"},
		{"-fs_game <gamename>", "Sets a different game directory name (up to 10)"},
		{"-fs_home <path>", "Sets user home path on Linux"},
		{"-fs_homebase <dir>", "Specifies where the user home directory name is on Linux (default for Q3A: .q3a)"},
		{"-fs_homepath <path>", "Sets the given path as home directory name"},
		{"-fs_nobasepath", "Do not load base paths in VFS"},
		{"-fs_nohomepath", "Do not load home path in VFS"},
		{"-fs_pakpath <path>", "Specify a package directory (up to 200)"},
		{"-game <gamename>", "Load settings for the given game (default: quake3)"},
		{"-help <stage>", "Prints this information"},
		{"-subdivisions <N>", "Patch mesh subdivision amount"},
		{"-threads <N>", "Limit CPU usage to maximum usage of N threads"},
		{"-v", "Verbose mode"}
	};

	HelpOptions("Common Options", 0, 100, common, sizeof(common)/sizeof(struct HelpOption));

}

void HelpMain(const char* arg)
{
	printf("Usage: q3map2 [stage] [common options...] [stage options...] [stage source file]\n");
	printf("       q3map2 -help [stage]\n\n");

	HelpCommon();

	struct HelpOption stages[] = {
		{"-bsp", "BSP Stage"},
		{"-vis", "VIS Stage"},
		{"-light", "Light Stage"},
		{"-analyze", "Analyzing BSP-like file structure"},
		{"-scale", "Scaling"},
		{"-convert", "Converting & Decompiling"},
		{"-export", "Exporting lightmaps"},
		{"-exportents", "Exporting entities"},
		{"-fixaas", "Fixing AAS checksum"},
		{"-info", "Get info about BSP file"},
		{"-import", "Importing lightmaps"},
		{"-minimap", "MiniMap"},
	};
	void(*help_funcs[])() = {
		HelpBsp,
		HelpVis,
		HelpLight,
		HelpAnalyze,
		HelpScale,
		HelpConvert,
		HelpExport,
		HelpExportEnts,
		HelpFixaas,
		HelpInfo,
		HelpImport,
		HelpMinimap,
	};

	if ( arg && strlen(arg) > 0 )
	{
		if ( arg[0] == '-' )
			arg++;

		unsigned i;
		for ( i = 0; i < sizeof(stages)/sizeof(struct HelpOption); i++ )
			if ( strcmp(arg, stages[i].name+1) == 0 )
			{
				help_funcs[i]();
				return;
			}
	}

	HelpOptions("Stages", 0, 100, stages, sizeof(stages)/sizeof(struct HelpOption));
}
