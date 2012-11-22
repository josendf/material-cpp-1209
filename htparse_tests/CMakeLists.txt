cmake_minimum_required( VERSION 2.8 )

project( htparse_tests CXX )

include_directories(
  "${htparse_SOURCE_DIR}/include"
  "${ext_SOURCE_DIR}/include"
  "${WORKSPACE_USR_DIR}/include" )

link_directories(
  "${WORKSPACE_USR_DIR}/lib" )

set( PROJECT_SRC_FILES
  src/precompiled.cpp
  src/test_config.cpp
  src/test_main.cpp
  src/filter_test.cpp
  src/hmac_test.cpp
  src/message_test.cpp
  src/on_error_test.cpp
  src/parser_test.cpp
  src/path_test.cpp
  src/rheader_test.cpp
  src/status_test.cpp
  )

if( CMAKE_COMPILER_IS_GNUCC )

  set( PROJECT_LIBS
    htparse
    boost_unit_test_framework
    boost_log
    boost_regex
    boost_thread
    boost_filesystem
    boost_date_time
    boost_locale
    boost_system
    icuuc
    icudata
    zip
    )

elseif( MSVC )

  set( PROJECT_LIBS
    htparse
    icuin
    icuuc
    icudt
    zip
    Gdiplus
    Version
    Rpcrt4
    )

endif()

add_executable( htparse_tests ${PROJECT_SRC_FILES} )

add_dependencies( htparse_tests
  htparse
  zip
  )

target_link_libraries( htparse_tests  ${PROJECT_LIBS} )

HelpersSetupTest( htparse_tests )

add_test( NAME htparse_tests COMMAND htparse_tests )

set_tests_properties( htparse_tests PROPERTIES
  PASS_REGULAR_EXPRESSION "${TESTING_REGEXP}"
  ENVIRONMENT "${TESTING_ENVIRONMENT}")
