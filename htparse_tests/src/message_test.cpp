#include "precompiled.h"
#include "test_config.hpp"
#include <htparse/backends/testing.hpp>

BOOST_AUTO_TEST_SUITE( message_test )


BOOST_AUTO_TEST_CASE( construction )
{
  using htparse::context;
  using htparse::message;
  using htparse::context_ptr;

  context_ptr ctx = context::make();

  message m1(ctx);
  BOOST_CHECK( m1.status().empty() );
  BOOST_CHECK( m1.status().code() == 0 );
  BOOST_CHECK( m1.status().message().empty() );
  BOOST_CHECK( !m1.status().has_error() );
}

BOOST_AUTO_TEST_CASE( set_status )
{
  using htparse::context;
  using htparse::message;
  using htparse::context_ptr;
  using htparse::status;

  context_ptr ctx = context::make();

  message m1(ctx);

  message m2 = m1.set_status(400, "Bad Request");

  BOOST_CHECK( m1.status().empty() );
  BOOST_CHECK( m1.status().code() == 0 );
  BOOST_CHECK( m1.status().message().empty() );
  BOOST_CHECK( !m1.status().has_error() );

  BOOST_CHECK( !m2.status().empty() );
  BOOST_CHECK( m2.status().code() == 400 );
  BOOST_CHECK( m2.status().message() == "Bad Request" );
  BOOST_CHECK( m2.status().has_error() );

  message m3 = m1.set_status([](){ return status(404, "Not Found"); });

  BOOST_CHECK( m1.status().empty() );
  BOOST_CHECK( m1.status().code() == 0 );
  BOOST_CHECK( m1.status().message().empty() );
  BOOST_CHECK( !m1.status().has_error() );

  BOOST_CHECK( !m3.status().empty() );
  BOOST_CHECK( m3.status().code() == 404 );
  BOOST_CHECK( m3.status().message() == "Not Found" );
  BOOST_CHECK( m3.status().has_error() );

}

BOOST_AUTO_TEST_CASE( output )
{
  using htparse::context;
  using htparse::message;
  using htparse::context_ptr;
  using htparse::status;

  context_ptr ctx = context::make();

  message m1(ctx);

  message m2 = m1.output("text/plain", "abc");
  BOOST_CHECK( !m1.has_response_content_type() );
  BOOST_CHECK( m1.response_body().empty() );

  BOOST_CHECK( m2.has_response_content_type("text/plain") );
  BOOST_CHECK( m2.response_body() == "abc" );

  message m3 = m2.output("", "def");
  BOOST_CHECK( !m1.has_response_content_type() );
  BOOST_CHECK( m1.response_body().empty() );

  BOOST_CHECK( m2.has_response_content_type("text/plain") );
  BOOST_CHECK( m2.response_body() == "abc" );

  BOOST_CHECK( m3.has_response_content_type("text/plain") );
  BOOST_CHECK( m3.response_body() == "abcdef" );
}

BOOST_AUTO_TEST_CASE( output_status )
{
  using htparse::context;
  using htparse::message;
  using htparse::context_ptr;
  using htparse::status;

  context_ptr ctx = context::make();

  message m1(ctx);

  message m2 = m1.output_status(200, "OK", "text/plain", "abc");
  BOOST_CHECK( m1.status().empty() );
  BOOST_CHECK( m1.status().code() == 0 );
  BOOST_CHECK( m1.status().message().empty() );
  BOOST_CHECK( !m1.status().has_error() );
  BOOST_CHECK( !m1.has_response_content_type() );
  BOOST_CHECK( m1.response_body().empty() );

  BOOST_CHECK( !m2.status().empty() );
  BOOST_CHECK( m2.status().code() == 200 );
  BOOST_CHECK( m2.status().message() == "OK" );
  BOOST_CHECK( !m2.status().has_error() );
  BOOST_CHECK( m2.has_response_content_type("text/plain") );
  BOOST_CHECK( m2.response_body() == "abc" );
}

BOOST_AUTO_TEST_CASE( copy )
{
  using htparse::context;
  using htparse::message;
  using htparse::context_ptr;
  using htparse::status;

  context_ptr ctx = context::make();

  message m1(ctx);

  message m2_ = m1.set_status(400, "Bad Request");
  message m2 = m2_;

  BOOST_CHECK( m1.status().empty() );
  BOOST_CHECK( m1.status().code() == 0 );
  BOOST_CHECK( m1.status().message().empty() );
  BOOST_CHECK( !m1.status().has_error() );

  BOOST_CHECK( !m2.status().empty() );
  BOOST_CHECK( m2.status().code() == 400 );
  BOOST_CHECK( m2.status().message() == "Bad Request" );
  BOOST_CHECK( m2.status().has_error() );

  message m3_ = m1.output("text/plain", "abc");
  message m3 = m3_;
  BOOST_CHECK( !m1.has_response_content_type() );
  BOOST_CHECK( m1.response_body().empty() );

  BOOST_CHECK( m3.has_response_content_type("text/plain") );
  BOOST_CHECK( m3.response_body() == "abc" );

}

BOOST_AUTO_TEST_SUITE_END()
