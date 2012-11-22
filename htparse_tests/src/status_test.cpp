#include "precompiled.h"
#include "test_config.hpp"
#include <htparse/backends/testing.hpp>

BOOST_AUTO_TEST_SUITE( status_test )

BOOST_AUTO_TEST_CASE( construction )
{
  using htparse::status;
  status v1;
  BOOST_CHECK( v1.empty() );
  BOOST_CHECK( v1.code() == 0 );
  BOOST_CHECK( v1.message().empty() );
  BOOST_CHECK( !v1.has_error() );

  status v2(0, "");
  BOOST_CHECK( v2.empty() );
  BOOST_CHECK( v2.code() == 0 );
  BOOST_CHECK( v2.message().empty() );
  BOOST_CHECK( !v2.has_error() );

  status v3(0, "abc");
  BOOST_CHECK( v3.empty() );
  BOOST_CHECK( v3.code() == 0 );
  BOOST_CHECK( !v3.message().empty() );
  BOOST_CHECK( !v3.has_error() );

  status v4(1, "");
  BOOST_CHECK( !v4.empty() );
  BOOST_CHECK( v4.code() == 1 );
  BOOST_CHECK( v4.message().empty() );
  BOOST_CHECK( !v4.has_error() );

  status v5(1, "abc");
  BOOST_CHECK( !v5.empty() );
  BOOST_CHECK( v5.code() == 1 );
  BOOST_CHECK( !v5.message().empty() );
  BOOST_CHECK( !v5.has_error() );

  status v6(400, "Bad Request");
  BOOST_CHECK( !v6.empty() );
  BOOST_CHECK( v6.code() == 400 );
  BOOST_CHECK( v6.message() == "Bad Request" );
  BOOST_CHECK( v6.has_error() );

  status v7(200, "OK");
  BOOST_CHECK( !v7.empty() );
  BOOST_CHECK( v7.code() == 200 );
  BOOST_CHECK( v7.message() == "OK" );
  BOOST_CHECK( !v7.has_error() );
}

BOOST_AUTO_TEST_CASE( copy )
{
  using htparse::status;
  status v1_;
  status v1 = v1_;
  BOOST_CHECK( v1.empty() );
  BOOST_CHECK( v1.code() == 0 );
  BOOST_CHECK( v1.message().empty() );
  BOOST_CHECK( !v1.has_error() );

  status v2_(0, "");
  status v2 = v2_;
  BOOST_CHECK( v2.empty() );
  BOOST_CHECK( v2.code() == 0 );
  BOOST_CHECK( v2.message().empty() );
  BOOST_CHECK( !v2.has_error() );

  status v3_(0, "abc");
  status v3 = v3_;
  BOOST_CHECK( v3.empty() );
  BOOST_CHECK( v3.code() == 0 );
  BOOST_CHECK( !v3.message().empty() );
  BOOST_CHECK( !v3.has_error() );

  status v4_(1, "");
  status v4 = v4_;
  BOOST_CHECK( !v4.empty() );
  BOOST_CHECK( v4.code() == 1 );
  BOOST_CHECK( v4.message().empty() );
  BOOST_CHECK( !v4.has_error() );

  status v5_(1, "abc");
  status v5 = v5_;
  BOOST_CHECK( !v5.empty() );
  BOOST_CHECK( v5.code() == 1 );
  BOOST_CHECK( !v5.message().empty() );
  BOOST_CHECK( !v5.has_error() );

  status v6_(400, "Bad Request");
  status v6 = v6_;
  BOOST_CHECK( !v6.empty() );
  BOOST_CHECK( v6.code() == 400 );
  BOOST_CHECK( v6.message() == "Bad Request" );
  BOOST_CHECK( v6.has_error() );
}

BOOST_AUTO_TEST_SUITE_END()
