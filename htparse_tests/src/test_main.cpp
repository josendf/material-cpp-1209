#include "precompiled.h"
#if !defined(_MSC_VER)
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MAIN
#include "test_config.hpp"

struct GlobalFixture {

  GlobalFixture()
  {
    test_config::initialize();
  }

  ~GlobalFixture()
  {
    test_config::terminate();
  }
};

BOOST_GLOBAL_FIXTURE(GlobalFixture)
