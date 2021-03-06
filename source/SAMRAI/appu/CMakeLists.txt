set (appu_headers
  BoundaryUtilityStrategy.h
  CartesianBoundaryDefines.h
  CartesianBoundaryUtilities2.h
  CartesianBoundaryUtilities3.h
  VisDerivedDataStrategy.h
  VisItDataWriter.h
  VisMaterialsDataStrategy.h)

set (appu_sources
  BoundaryUtilityStrategy.C
  CartesianBoundaryUtilities2.C
  CartesianBoundaryUtilities3.C
  VisDerivedDataStrategy.C
  VisItDataWriter.C
  VisMaterialsDataStrategy.C
  fortran/appu_cpdat2buf2d.f
  fortran/appu_cpdat2buf3d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/appu_cartbdry2d.f
  ${CMAKE_CURRENT_BINARY_DIR}/fortran/appu_cartbdry3d.f)

set (appu_depends_on
  SAMRAI_hier
  SAMRAI_pdat
  SAMRAI_tbox)

if (ENABLE_HDF5)
  set (appu_depends_on
    hdf5
    ${appu_depends_on})
endif ()

process_m4(NAME fortran/appu_cartbdry2d)
process_m4(NAME fortran/appu_cartbdry3d)

blt_add_library(
  NAME SAMRAI_appu
  SOURCES ${appu_sources}
  HEADERS ${appu_headers}
  DEPENDS_ON ${appu_depends_on})

set_target_properties(SAMRAI_appu PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories( SAMRAI_appu
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
  $<INSTALL_INTERFACE:include>)

install(TARGETS SAMRAI_appu
  EXPORT SAMRAITargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ${appu_headers}
  DESTINATION include/SAMRAI/appu)
