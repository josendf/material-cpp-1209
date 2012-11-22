cmake_minimum_required( VERSION 2.8 )

project( htparse CXX )

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${ext_SOURCE_DIR}/include"
  "${WORKSPACE_USR_DIR}/include"
  )

set( PROJECT_INCLUDE_FILES
  include/htparse/auth/mac.hpp
  include/htparse/auth/bearer.hpp
  include/htparse/backends/cppcms.hpp
  include/htparse/backends/cppcms/backend.hpp
  include/htparse/backends/cppcms/context.hpp
  include/htparse/backends/cppcms/request.hpp
  include/htparse/backends/cppcms/response.hpp
  include/htparse/backends/cppcms/types.hpp
  include/htparse/backends/testing.hpp
  include/htparse/backends/testing/backend.hpp
  include/htparse/backends/testing/context.hpp
  include/htparse/backends/testing/request.hpp
  include/htparse/backends/testing/response.hpp
  include/htparse/backends/testing/result.hpp
  include/htparse/backends/testing/types.hpp
  include/htparse/evaluator.hpp
  include/htparse/filter.hpp
  include/htparse/filters/auth.hpp
  include/htparse/filters/choice.hpp
  include/htparse/filters/content.hpp
  include/htparse/filters/header.hpp
  include/htparse/filters/bearer_auth.hpp
  include/htparse/filters/mac_auth.hpp
  include/htparse/filters/method.hpp
  include/htparse/filters/on_error.hpp
  include/htparse/filters/option.hpp
  include/htparse/filters/output.hpp
  include/htparse/filters/params.hpp
  include/htparse/filters/pass.hpp
  include/htparse/filters/path.hpp
  include/htparse/filters/rheader.hpp
  include/htparse/filters.hpp
  include/htparse/frontend.hpp
  include/htparse/helpers.hpp
  include/htparse/message.hpp
  include/htparse/parser.hpp
  include/htparse/status.hpp
  include/htparse/types.hpp
  )

set( PROJECT_SRC_FILES
  src/precompiled.cpp
  )

add_library( htparse ${PROJECT_SRC_FILES} )

add_dependencies( htparse ext )

HelpersSetupLibrary( htparse )

install( TARGETS htparse DESTINATION lib )

install( FILES ${PROJECT_INCLUDE_FILES} DESTINATION "include/htparse" )
