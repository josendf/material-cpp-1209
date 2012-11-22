cmake_minimum_required( VERSION 2.8 )

project( statsc CXX )

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${ext_SOURCE_DIR}/include"
  "${WORKSPACE_USR_DIR}/include"
  )

set( PROJECT_INCLUDE_FILES
  include/statsc/statsd_client.hpp
  include/statsc/statsd_metric.hpp
  include/statsc/statsd.hpp
  )

set( PROJECT_SRC_FILES
  src/precompiled.cpp
  src/statsd_client.cpp
  src/statsd_metric.cpp
  )

if(PLATFORM_WINDOWS)
elseif(PLATFORM_LINUX)
elseif(PLATFORM_MACOSX)
endif()

add_library( statsc ${PROJECT_SRC_FILES} )

add_dependencies( statsc ext )

HelpersSetupLibrary( statsc )

install( TARGETS statsc DESTINATION lib )

install( FILES ${PROJECT_INCLUDE_FILES} DESTINATION "include/statsc" )
