set (linadv_sources
  LinAdv.C
  main.C
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/flux2d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/flux3d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/fluxcorner3d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/grad2d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/grad3d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/init2d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/init3d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/stable2d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/stable3d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/stufprobc.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/trace1d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/trace2d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/trace3d.f)

process_m4(NAME fortran/flux2d)
process_m4(NAME fortran/flux3d)
process_m4(NAME fortran/fluxcorner3d)
process_m4(NAME fortran/grad2d)
process_m4(NAME fortran/grad3d)
process_m4(NAME fortran/init2d)
process_m4(NAME fortran/init3d)
process_m4(NAME fortran/stable2d)
process_m4(NAME fortran/stable3d)
process_m4(NAME fortran/stufprobc)
process_m4(NAME fortran/trace1d)
process_m4(NAME fortran/trace2d)
process_m4(NAME fortran/trace3d)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTESTING=1") 

blt_add_executable(
  NAME linadv
  SOURCES ${linadv_sources}
  DEPENDS_ON
    testlib
    SAMRAI_algs
    SAMRAI_appu
    SAMRAI_geom
    SAMRAI_hier
    SAMRAI_math
    SAMRAI_mesh
    SAMRAI_pdat
    SAMRAI_tbox
    SAMRAI_xfer)

target_include_directories( linadv
  PUBLIC ${PROJECT_SOURCE_DIR}/source/test/applications/LinAdv)

set_target_properties(linadv PROPERTIES LINKER_LANGUAGE CXX)

file (GLOB test_inputs ${CMAKE_CURRENT_SOURCE_DIR}/test_inputs/*.input)
file (GLOB test_baselines ${CMAKE_CURRENT_SOURCE_DIR}/test_inputs/*boxes*)

execute_process(COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/test_inputs)
execute_process(COMMAND ln -s ${test_baselines} ${CMAKE_CURRENT_BINARY_DIR}/test_inputs)

samrai_add_tests(
  NAME linadv
  EXECUTABLE linadv
  INPUTS ${test_inputs}
  PARALLEL TRUE)

