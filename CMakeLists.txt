cmake_minimum_required(VERSION 3.22)
project(3DMMForever LANGUAGES CXX C VERSION 0.2)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_CXX_STANDARD 14)

if(MSVC)
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
  # permissive forces C++14 const char* as default for literals
  # this flag re-enables the auto-conversion for the microsoft compiler
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:strictStrings-")
endif()

include(CMakeDependentOption)
include(TargetChompSources)

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
  find_package(AudioMan)
endif()
# find_package(BRender REQUIRED)

# Options
option(BUILD_PACKAGES "Build packages for 3DMM" ON)

cmake_dependent_option(3DMM_PACKAGE_WIX "Generate an MSI via WiX" ON "BUILD_PACKAGES AND DEFINED ENV{WIX}" OFF)
cmake_dependent_option(3DMM_PACKAGE_ZIP "Generate a portable ZIP" ON "BUILD_PACKAGES" OFF)

# Optional Tools
find_package(ClangTidy)
find_package(CCache)

if (TARGET CCache::CCache AND NOT CMAKE_CXX_COMPILER_LAUNCHER)
  get_property(CMAKE_CXX_COMPILER_LAUNCHER TARGET CCache::CCache PROPERTY IMPORTED_LOCATION)
  get_property(CMAKE_C_COMPILER_LAUNCHER TARGET CCache::CCache PROPERTY IMPORTED_LOCATION)
endif()

# TODO: Enable this when the 
#if (TARGET Clang::Tidy AND NOT CMAKE_CXX_CLANG_TIDY)
#  get_property(CMAKE_CXX_CLANG_TIDY TARGET Clang::Tidy PROPERTY IMPORTED_LOCATION)
#endif()

# Local Tooling
add_compile_definitions(
  $<$<PLATFORM_ID:Windows>:WIN>
  $<$<CONFIG:Debug>:DEBUG>
  LITTLE_ENDIAN
)

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
  add_compile_definitions(
    $<$<PLATFORM_ID:Windows>:IN_80386>
  )
endif()

# NOTE: kcdc-386 and kcd2-386 prevent cross compiling at this time.
add_executable(kcdc-386 EXCLUDE_FROM_ALL)
target_sources(kcdc-386 PRIVATE "${PROJECT_SOURCE_DIR}/kauai/src/kcdc_386.c")
target_include_directories(kcdc-386 PRIVATE $<TARGET_PROPERTY:kauai,INCLUDE_DIRECTORIES>)

add_executable(kcd2-386 EXCLUDE_FROM_ALL)
target_sources(kcd2-386 PRIVATE "${PROJECT_SOURCE_DIR}/kauai/src/kcd2_386.c")
target_include_directories(kcd2-386 PRIVATE $<TARGET_PROPERTY:kauai,INCLUDE_DIRECTORIES>)

if (NOT TARGET 3DMMForever::AudioMan)
  add_library(audioman)
  add_library(3DMMForever::AudioMan ALIAS audioman)
  target_include_directories(audioman PUBLIC
      "${PROJECT_SOURCE_DIR}/kauai/src"
  )
  target_sources(audioman PRIVATE
      "${PROJECT_SOURCE_DIR}/audioman/audioman.cpp"
  )
endif()

add_executable(chelp WIN32 EXCLUDE_FROM_ALL)
target_sources(chelp PRIVATE
    "${PROJECT_SOURCE_DIR}/kauai/tools/chtop.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chelp.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chelpexp.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chelp.rc"
)
target_include_directories(chelp PRIVATE $<TARGET_PROPERTY:kauai,INCLUDE_DIRECTORIES>)
target_link_libraries(chelp PRIVATE $<TARGET_NAME_IF_EXISTS:kauai>)

# Kauai test applications
add_executable(ft WIN32 EXCLUDE_FROM_ALL)
target_sources(ft PRIVATE
    "${PROJECT_SOURCE_DIR}/kauai/src/test.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/ft.rc"
    "${PROJECT_SOURCE_DIR}/kauai/src/ft.cpp"
)
target_link_libraries(ft PRIVATE kauai)

add_executable(ut EXCLUDE_FROM_ALL)
target_sources(ut PRIVATE
    "${PROJECT_SOURCE_DIR}/kauai/src/ut.cpp"
)
target_link_libraries(ut PRIVATE kauai)

add_executable(chomp EXCLUDE_FROM_ALL)
target_sources(chomp PRIVATE "${PROJECT_SOURCE_DIR}/kauai/tools/chomp.cpp")
target_include_directories(chomp PRIVATE $<TARGET_PROPERTY:kauai,INCLUDE_DIRECTORIES>)
target_link_libraries(chomp PRIVATE $<TARGET_NAME_IF_EXISTS:kauai>)

# Chunk Editor
add_executable(ched WIN32 EXCLUDE_FROM_ALL)
target_sources(ched PRIVATE
    "${PROJECT_SOURCE_DIR}/kauai/tools/ched.rc"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chdoc.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/ched.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chgrp.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chhex.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chmbmp.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chpic.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chtxt.cpp"
)
target_link_libraries(ched PRIVATE kauai)

# mkmbmp
add_executable(mkmbmp EXCLUDE_FROM_ALL)
target_sources(mkmbmp PRIVATE
    "${PROJECT_SOURCE_DIR}/kauai/tools/mkmbmp.cpp"
)
target_link_libraries(mkmbmp PRIVATE kauai)

# kpack
add_executable(kpack EXCLUDE_FROM_ALL)
target_sources(kpack PRIVATE
    "${PROJECT_SOURCE_DIR}/kauai/tools/kpack.cpp"
)
target_link_libraries(kpack PRIVATE kauai)

# chmerge
add_executable(chmerge EXCLUDE_FROM_ALL)
target_sources(chmerge PRIVATE
    "${PROJECT_SOURCE_DIR}/kauai/tools/chmerge.cpp"
)
target_link_libraries(chmerge PRIVATE kauai)

# chelpdmp
add_executable(chelpdmp EXCLUDE_FROM_ALL)
target_sources(chelpdmp PRIVATE
    "${PROJECT_SOURCE_DIR}/kauai/tools/chelpdmp.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/tools/chelpexp.cpp"
)
target_link_libraries(chelpdmp PRIVATE kauai)

# These are only here to ensure configure is rerun when dependencies are generated.
# This is 
file(GLOB building-chunk-sources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/building/*.cht")
file(GLOB studio-chunk-sources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/studio/*.cht")

add_custom_command(
  OUTPUT "${PROJECT_BINARY_DIR}/generated/kauai/src/kcdc_386.h"
  COMMAND cmake -E make_directory "${PROJECT_BINARY_DIR}/generated/kauai/src"
  COMMAND kcdc-386 "${PROJECT_BINARY_DIR}/generated/kauai/src/kcdc_386.h"
  COMMENT "Generating kcdc_386.h"
  VERBATIM
)

add_custom_command(
  OUTPUT "${PROJECT_BINARY_DIR}/generated/kauai/src/kcd2_386.h"
  COMMAND cmake -E make_directory "${PROJECT_BINARY_DIR}/generated/kauai/src"
  COMMAND kcd2-386 "${PROJECT_BINARY_DIR}/generated/kauai/src/kcd2_386.h"
  COMMENT "Generating kcd2_386.h"
  VERBATIM
)

add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/wd4430>)
add_library(kauai)
target_include_directories(
  kauai
    PUBLIC
      "${PROJECT_SOURCE_DIR}/kauai/src"
      "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated/kauai/src>")

target_sources(kauai
  PRIVATE
    "${PROJECT_BINARY_DIR}/generated/kauai/src/kcdc_386.h"
    "${PROJECT_BINARY_DIR}/generated/kauai/src/kcd2_386.h"

    "${PROJECT_SOURCE_DIR}/kauai/src/appb.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/base.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/chcm.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/chse.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/chunk.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/clip.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/clok.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/cmd.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/codec.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/codkauai.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/crf.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/ctl.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/cursor.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/dlg.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/docb.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/file.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/frame.rc"
    "${PROJECT_SOURCE_DIR}/kauai/src/gfx.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/gob.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/groups.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/groups2.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/kidhelp.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/kidspace.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/kidworld.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/lex.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/mbmp.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/mbmpgui.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/midi.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/mididev.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/mididev2.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/mssio.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/pic.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/region.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/rtxt.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/rtxt2.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/scrcom.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/scrcomg.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/screxe.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/screxeg.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/sndam.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/sndm.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/spell.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/stream.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/text.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/textdoc.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/util.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/utilcopy.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/utilerro.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/utilglob.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/utilint.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/utilmem.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/utilrnd.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/utilstr.cpp"
    "${PROJECT_SOURCE_DIR}/kauai/src/video.cpp"

    # Windows implementations
    $<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/kauai/src/appbwin.cpp>
    $<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/kauai/src/dlgwin.cpp>
    $<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/kauai/src/filewin.cpp>
    $<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/kauai/src/fniwin.cpp>
    $<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/kauai/src/gfxwin.cpp>
    $<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/kauai/src/memwin.cpp>
    $<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/kauai/src/menuwin.cpp>
    $<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/kauai/src/picwin.cpp>
    $<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/kauai/src/gobwin.cpp>

    # Stubs for Visual C++ 2.1 CRT functions
    "${PROJECT_SOURCE_DIR}/kauai/src/stub.cpp"   
)

target_compile_definitions(kauai PUBLIC
  _LPCVOID_DEFINED
  STRICT)

target_link_libraries(kauai
  PUBLIC
    3DMMForever::AudioMan
    $<$<PLATFORM_ID:Windows>:Msacm32>
    $<$<PLATFORM_ID:Windows>:Vfw32>
    $<$<PLATFORM_ID:Windows>:Winmm>
    $<$<PLATFORM_ID:Windows>:mpr>)

add_library(brender)
target_sources(brender PRIVATE
  "${PROJECT_SOURCE_DIR}/bren/bwld.cpp"
  "${PROJECT_SOURCE_DIR}/bren/stderr.c"
  "${PROJECT_SOURCE_DIR}/bren/stdfile.c"
  "${PROJECT_SOURCE_DIR}/bren/stdmem.c"
  "${PROJECT_SOURCE_DIR}/bren/tmap.cpp"
  "${PROJECT_SOURCE_DIR}/bren/zbmp.cpp"
    
  "${PROJECT_SOURCE_DIR}/bren/FW/actsupt.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/angles.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/brlists.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/brqsort.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/bswap.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/custsupt.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/datafile.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/def_mdl.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/def_mat.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/diag.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/envmap.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/error.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/ffhooks.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/file.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/fileops.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/fixed.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/fontptrs.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/fwsetup.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/light8.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/light8o.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/light24.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/light24o.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/logprint.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/matrix23.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/matrix34.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/matrix4.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/mem.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/onscreen.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/pick.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/pixelmap.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/pmdsptch.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/pmgenops.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/pmmemops.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/pool.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/prelight.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/prepmatl.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/prepmesh.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/prepmap.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/preptab.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/quat.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/quantize.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/register.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/regsupt.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/resource.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/scalar.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/scale.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/scratch.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/scrstr.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/surface.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/transfrm.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/vector.c"
  
  "${PROJECT_SOURCE_DIR}/bren/FW/fixed_agnostic.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/blockops.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/font_f_3x5.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/font_p_4x6.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/font_p_7x9.c"
  "${PROJECT_SOURCE_DIR}/bren/FW/memloops.c"

	"${PROJECT_SOURCE_DIR}/bren/ZB/awtmz.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/bbox.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/decalz.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/dither.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/frcp.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/l_piz.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/p_piz.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/perspz.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/zbclip.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/zbmatl.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/zbmesh.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/zbmeshe.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/zbmeshp.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/zbrendr.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/zbsetup.c"

	"${PROJECT_SOURCE_DIR}/bren/ZB/mesh_agnostic.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/safediv.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/t_piza.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/ti8_pizp.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/ti8_piz.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/tt15_piz.c"
	"${PROJECT_SOURCE_DIR}/bren/ZB/tt24_piz.c"
)
target_include_directories(brender PUBLIC
  "${PROJECT_SOURCE_DIR}/BREN/INC"
  "${PROJECT_SOURCE_DIR}/bren/FW"
)
if (CMAKE_C_BYTE_ORDER MATCHES BIG_ENDIAN)
	target_compile_definitions (brender PUBLIC "BR_ENDIAN_BIG=1")
else()
  target_compile_definitions (brender PUBLIC "BR_ENDIAN_BIG=0")
endif()
if (CMAKE_C_BYTE_ORDER MATCHES LITTLE_ENDIAN)
  target_compile_definitions (brender PUBLIC "BR_ENDIAN_LITTLE=1")
else()
  target_compile_definitions (brender PUBLIC "BR_ENDIAN_LITTLE=0")
endif ()
target_link_libraries(brender PUBLIC
  kauai
  #BRender::Libraries
)

add_library(engine)
target_sources(engine
    PRIVATE
    "${PROJECT_SOURCE_DIR}/src/engine/actor.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/actredit.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/actrsave.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/actrsnd.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/bkgd.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/body.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/modl.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/movie.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/msnd.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/mtrl.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/scene.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/srec.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/tagl.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/tagman.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/tbox.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/tdf.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/tdt.cpp"
    "${PROJECT_SOURCE_DIR}/src/engine/tmpl.cpp"
)
target_include_directories(engine PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/INC>)
target_link_libraries(engine PUBLIC kauai brender)

# On non-windows WIN32 is a no-op
add_executable(studio WIN32)
target_sources(studio
    PRIVATE
    "${PROJECT_SOURCE_DIR}/src/studio/ape.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/browser.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/esl.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/mminstal.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/popup.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/portf.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/scnsort.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/splot.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/stdiobrw.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/stdioscb.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/studio.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/tatr.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/tgob.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/utest.cpp"
    "${PROJECT_SOURCE_DIR}/src/studio/utest.rc"
    "${PROJECT_SOURCE_DIR}/src/studio/utest.rc2"
)
target_include_directories(studio
  PUBLIC $<TARGET_PROPERTY:engine,INCLUDE_DIRECTORIES>
  "${PROJECT_SOURCE_DIR}/src")
target_chomp_sources(studio
  "${PROJECT_SOURCE_DIR}/src/studio/utest.cht"
  "${PROJECT_SOURCE_DIR}/src/studio/studio.cht"
  "${PROJECT_SOURCE_DIR}/src/building/bldghd.cht"
  "${PROJECT_SOURCE_DIR}/src/building/building.cht"
  "${PROJECT_SOURCE_DIR}/src/helpaud/helpaud.cht"
  "${PROJECT_SOURCE_DIR}/src/help/help.cht"
  "${PROJECT_SOURCE_DIR}/src/shared/shared.cht"
  "${PROJECT_SOURCE_DIR}/src/shared/sharecd.cht")

target_link_libraries(studio PRIVATE engine)
set_property(TARGET studio PROPERTY OUTPUT_NAME 3dmovie)
target_link_options(studio BEFORE PRIVATE $<$<LINK_LANG_AND_ID:CXX,MSVC>:/MANIFESTUAC:NO>)
target_link_options(studio
  BEFORE PRIVATE
    $<$<AND:$<LINK_LANG_AND_ID:CXX,MSVC>,$<CONFIG:DEBUG>>:/NODEFAULTLIB:libcmt.lib>)

set_property(
    TARGET studio
    PROPERTY 3DMM_SAMPLES
    ${studio-samples}
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/BONGO.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/BOOOOOO.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/CITYTOUR.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/GRAVEYRD.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/HAUNTED.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/HOSPITAL.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/JUNGLE.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/MESSAGE.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/SPACE.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/SPROG.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/TERROR.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/THEBOOK.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/THELODGE.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/THETHIEF.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/VENUS31.3MM"
    "${PROJECT_SOURCE_DIR}/cd3/SAMPLES/WHERE.3MM"
)

# Please pay attention to the ending / in each DIRECTORY declaration. It
# affects the destination!
install(TARGETS studio RUNTIME DESTINATION .)
# HACK(bruxisma): This is due to UTEST.CHT not generating 3DMOVIE.CHK
#                 Renaming it to 3DMOVIE.CHT would resolve this.
install(FILES $<FILTER:$<TARGET_PROPERTY:studio,CHOMPED_CHUNKS>,EXCLUDE,.+UTEST[.]chk$>
  DESTINATION "Microsoft Kids/3D Movie Maker")
install(FILES $<FILTER:$<TARGET_PROPERTY:studio,CHOMPED_CHUNKS>,INCLUDE,.+UTEST[.]chk$>
  RENAME 3dmovie.chk
  DESTINATION "Microsoft Kids/3D Movie Maker")
install(FILES $<TARGET_PROPERTY:studio,3DMM_SAMPLES>
  DESTINATION "Microsoft Kids/Users/Melanie")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/cd9/"
  DESTINATION "Microsoft Kids/3D Movie Maker")

# CPack must be included *last*, which is antithetical to how most CMake
# Modules work. We can set all variables for all packaging regardless of
# whether they're supported. This way we can just swap out what is added to
# `CPACK_GENERATOR`

# DO NOT CHANGE THIS. EVER. Generated manually via uuidgen. This needs to be
# the same so that user's can automatically upgrade via MSI
set(CPACK_WIX_UPGRADE_GUID 01c5fef4-82b5-4194-aaf3-bd0247004941)
# Change this as needed.
set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT https://github.com/foone/3DMMForever)

if (BUILD_PACKAGES)
  add_custom_target(dist DEPENDS $<TARGET_NAME_IF_EXISTS:package>)
  foreach (generator IN ITEMS ZIP WIX)
    if (3DMM_PACKAGE_${generator})
      list(APPEND CPACK_GENERATOR ${generator})
    endif()
  endforeach()
  include(CPack)
endif()
