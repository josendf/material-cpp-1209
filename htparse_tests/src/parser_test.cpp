#include "precompiled.h"
#include "test_config.hpp"
#include <htparse/backends/testing.hpp>

BOOST_AUTO_TEST_SUITE( parser_test )


BOOST_AUTO_TEST_CASE( construction )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::parser_failed;
  using htparse::parser_complete;

  context_ptr ctx = context::make();
  message m1(ctx);

  parser p1(m1);
  BOOST_CHECK( !p1.failed() );
  BOOST_CHECK( !p1.complete() );
  BOOST_CHECK( !p1.failed_or_complete() );
  BOOST_CHECK( !p1.failed_not_complete() );

  parser p2(m1, parser_failed::failed );
  BOOST_CHECK( p2.failed() );
  BOOST_CHECK( !p2.complete() );
  BOOST_CHECK( p2.failed_or_complete() );
  BOOST_CHECK( p2.failed_not_complete() );

  parser p3(m1, parser_failed::failed, parser_complete::complete );
  BOOST_CHECK( p3.failed() );
  BOOST_CHECK( p3.complete() );
  BOOST_CHECK( p3.failed_or_complete() );
  BOOST_CHECK( !p3.failed_not_complete() );
}

BOOST_AUTO_TEST_CASE( copy )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::parser_failed;
  using htparse::parser_complete;

  context_ptr ctx = context::make();
  message m1(ctx);

  parser p1_(m1);
  parser p1 = p1_;
  BOOST_CHECK( !p1.failed() );
  BOOST_CHECK( !p1.complete() );
  BOOST_CHECK( !p1.failed_or_complete() );
  BOOST_CHECK( !p1.failed_not_complete() );

  parser p2_(m1, parser_failed::failed );
  parser p2 = p2_;
  BOOST_CHECK( p2.failed() );
  BOOST_CHECK( !p2.complete() );
  BOOST_CHECK( p2.failed_or_complete() );
  BOOST_CHECK( p2.failed_not_complete() );

  parser p3_(m1, parser_failed::failed, parser_complete::complete );
  parser p3 = p3_;
  BOOST_CHECK( p3.failed() );
  BOOST_CHECK( p3.complete() );
  BOOST_CHECK( p3.failed_or_complete() );
  BOOST_CHECK( !p3.failed_not_complete() );
}

BOOST_AUTO_TEST_CASE( fail )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::fail;

  context_ptr ctx = context::make();
  message m1(ctx);

  parser p1 = fail(m1, 400, "Bad Request");
  BOOST_CHECK( !m1.status().has_error() );
  BOOST_CHECK( m1.status().code() == 0 );
  BOOST_CHECK( m1.status().message().empty() );

  BOOST_CHECK( p1.failed() );
  BOOST_CHECK( !p1.complete() );
  BOOST_CHECK( p1.failed_or_complete() );
  BOOST_CHECK( p1.failed_not_complete() );
  BOOST_CHECK( p1.message().status().has_error() );
  BOOST_CHECK( p1.message().status().code() == 400 );
  BOOST_CHECK( p1.message().status().message() == "Bad Request" );

}

BOOST_AUTO_TEST_CASE( complete_with_error )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::complete_with_error;

  context_ptr ctx = context::make();
  message m1(ctx);

  parser p1 = complete_with_error(m1, 400, "Bad Request");
  BOOST_CHECK( !m1.status().has_error() );
  BOOST_CHECK( m1.status().code() == 0 );
  BOOST_CHECK( m1.status().message().empty() );

  BOOST_CHECK( !p1.failed() );
  BOOST_CHECK( p1.complete() );
  BOOST_CHECK( p1.failed_or_complete() );
  BOOST_CHECK( !p1.failed_not_complete() );
  BOOST_CHECK( p1.message().status().has_error() );
  BOOST_CHECK( p1.message().status().code() == 400 );
  BOOST_CHECK( p1.message().status().message() == "Bad Request" );

}

BOOST_AUTO_TEST_SUITE_END()
