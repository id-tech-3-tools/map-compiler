cmake_minimum_required (VERSION 3.8)



set(MAPCOMPILER_VERSION 0.2.0)
set(Q3MAP2_VERSION 2.5.17n)
add_definitions(-DMAPCOMPILER_VERSION="${MAPCOMPILER_VERSION}")
add_definitions(-DQ3MAP2_VERSION="${Q3MAP2_VERSION}")

include_directories(AFTER "${CMAKE_SOURCE_DIR}/third_party/assetsys")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/third_party/filesystem")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/third_party/jpeg-compressor")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/third_party/lodepng")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/third_party/msdirent")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/third_party/picomodel/src")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/third_party/picomodel/table-builder")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/third_party/tinyformat")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/third_party/table-builder")

file(GLOB headers *.h)

add_executable(mapcompiler 
    main.cpp
    assets_loader.cpp
    bbox.cpp
    brush.cpp
    brush_primit.cpp
    bsp_analyze.cpp
    bsp.cpp
    bspfile_abstract.cpp
    bspfile_ibsp.cpp
    bspfile_rbsp.cpp
    bsp_info.cpp
    bsp_scale.cpp
    cmdlib.cpp
    convert_ase.cpp
    convert_bsp.cpp
    convert_map.cpp
    convert_obj.cpp
    ddslib.cpp
    decals.cpp
    etclib.cpp
    exportents.cpp
    facebsp.cpp
    filematch.cpp
    fixaas.cpp
    fog.cpp
    help.cpp
    image.cpp
    imagelib.cpp
    inout.cpp
    leakfile.cpp
    light_bounce.cpp
    light.cpp
    lightmaps_ydnar.cpp
    light_trace.cpp
    light_ydnar.cpp
    line.cpp
    m4x4.cpp
    map.cpp
    mathlib.cpp
    md4.cpp
    md5lib.cpp
    mesh.cpp
    minimap.cpp
    model.cpp
    patch.cpp
    path_init.cpp
    polylib.cpp
    portals.cpp
    prtfile.cpp
    ray.cpp
    scriplib.cpp
    shaders.cpp
    surface.cpp
    surface_extra.cpp
    surface_foliage.cpp
    surface_fur.cpp
    surface_meta.cpp
    threads.cpp
    tjunction.cpp
    tree.cpp
    trilib.cpp
    vis.cpp
    visflow.cpp
    writebsp.cpp
    
    ${headers}
)

target_link_libraries(mapcompiler picomodel)
target_link_libraries(mapcompiler libtablebuilder)
target_link_libraries(mapcompiler liblodepng)
target_link_libraries(mapcompiler libjpegd)