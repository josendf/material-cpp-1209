#include "precompiled.h"
#include "test_config.hpp"
#include <htparse/backends/testing.hpp>

BOOST_AUTO_TEST_SUITE( filter_test )

BOOST_AUTO_TEST_CASE( fail )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::expr;
  using htparse::fail;
  using htparse::complete_with_error;
  using htparse::complete_with_status;
  using htparse::operator >=;

  context_ptr ctx = context::make();
  message m1(ctx);

  parser p1(m1);

  expr e1([](const message& m) {
      return fail(m);
    });

  expr e2([](const message& m) {
      return complete_with_status(m, 200, "OK", "text/plain", "abc");
    });

  parser p2 = p1 >= e1;
  message m2 = p2.message();
  BOOST_CHECK( !m1.has_response_content_type() );
  BOOST_CHECK( m1.response_body().empty() );

  BOOST_CHECK( p2.failed() );
  BOOST_CHECK( !p2.complete() );
  BOOST_CHECK( !m2.has_response_content_type() );
  BOOST_CHECK( m2.response_body().empty() );

  parser p3 = p1 >= e1 >= e2;
  message m3 = p3.message();
  BOOST_CHECK( p3.failed() );
  BOOST_CHECK( !p3.complete() );
  BOOST_CHECK( !m2.has_response_content_type() );
  BOOST_CHECK( m2.response_body().empty() );

  parser p4 = p1 >= e2 >= e1;
  message m4 = p4.message();
  BOOST_CHECK( !p4.failed() );
  BOOST_CHECK( p4.complete() );
  BOOST_CHECK( m4.has_response_content_type("text/plain") );
  BOOST_CHECK( m4.response_body() == "abc" );
}

BOOST_AUTO_TEST_CASE( complete_with_status )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::expr;
  using htparse::fail;
  using htparse::complete_with_error;
  using htparse::complete_with_status;
  using htparse::operator >=;

  context_ptr ctx = context::make();
  message m1(ctx);

  parser p1(m1);

  expr e1([](const message& m) {
      return complete_with_status(m, 200, "OK", "text/plain", "abc");
    });

  expr e2([](const message& m) {
      return complete_with_status(m, 200, "OK", "text/xml", "<a></a>");
    });

  parser p2 = p1 >= e1;
  message m2 = p2.message();
  BOOST_CHECK( !m1.has_response_content_type() );
  BOOST_CHECK( m1.response_body().empty() );

  BOOST_CHECK( !p2.failed() );
  BOOST_CHECK( p2.complete() );
  BOOST_CHECK( m2.has_response_content_type("text/plain") );
  BOOST_CHECK( m2.response_body() == "abc" );

  parser p3 = p1 >= e1 >= e2;
  message m3 = p3.message();
  BOOST_CHECK( !p3.failed() );
  BOOST_CHECK( p3.complete() );
  BOOST_CHECK( m3.has_response_content_type("text/plain") );
  BOOST_CHECK( m3.response_body() == "abc" );

  parser p4 = p1 >= e2 >= e1;
  message m4 = p4.message();
  BOOST_CHECK( !p4.failed() );
  BOOST_CHECK( p4.complete() );
  BOOST_CHECK( m4.has_response_content_type("text/xml") );
  BOOST_CHECK( m4.response_body() == "<a></a>" );
}

BOOST_AUTO_TEST_CASE( complete_with_error )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::expr;
  using htparse::fail;
  using htparse::complete_with_error;
  using htparse::complete_with_status;
  using htparse::operator >=;

  context_ptr ctx = context::make();
  message m1(ctx);

  parser p1(m1);

  expr e1([](const message& m) {
      return complete_with_status(m, 200, "OK", "text/plain", "abc");
    });

  expr e2([](const message& m) {
      return complete_with_error(m, 400, "Bad Request");
    });

  parser p2 = p1 >= e1;
  message m2 = p2.message();
  BOOST_CHECK( !m1.has_response_content_type() );
  BOOST_CHECK( m1.response_body().empty() );

  BOOST_CHECK( !p2.failed() );
  BOOST_CHECK( p2.complete() );
  BOOST_CHECK( m2.has_response_content_type("text/plain") );
  BOOST_CHECK( m2.response_body() == "abc" );

  parser p3 = p1 >= e1 >= e2;
  message m3 = p3.message();
  BOOST_CHECK( !p3.failed() );
  BOOST_CHECK( p3.complete() );
  BOOST_CHECK( m3.has_response_content_type("text/plain") );
  BOOST_CHECK( m3.response_body() == "abc" );

  parser p4 = p1 >= e2 >= e1;
  message m4 = p4.message();
  BOOST_CHECK( !p4.failed() );
  BOOST_CHECK( p4.complete() );
  BOOST_CHECK( m4.status().has_error() );
  BOOST_CHECK( m4.status().code() == 400 );
  BOOST_CHECK( m4.status().message() == "Bad Request" );
}

BOOST_AUTO_TEST_CASE( left_composition )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::expr;
  using htparse::fail;
  using htparse::complete_with_error;
  using htparse::complete_with_status;
  using htparse::operator >=;
  using htparse::operator >>;

  context_ptr ctx = context::make();
  message m1(ctx);

  parser p1(m1);

  expr e1([](const message& m) {
      return parse(m.output("", "abc"));
    });

  expr e2([](const message& m) {
      return parse(m.output("", "def"));
    });

  expr e3([](const message& m) {
      return parse(m.output("", "ghi"));
    });

  expr e4 = e1 >> e2;
  parser p4 = p1 >= e4;
  message m4 = p4.message();
  BOOST_CHECK( !p4.failed() );
  BOOST_CHECK( !p4.complete() );
  BOOST_CHECK( m4.response_body() == "abcdef" );

  expr e5 = e1 >> e2 >> e3;
  parser p5 = p1 >= e5;
  message m5 = p5.message();
  BOOST_CHECK( !p5.failed() );
  BOOST_CHECK( !p5.complete() );
  BOOST_CHECK( m5.response_body() == "abcdefghi" );

}

BOOST_AUTO_TEST_CASE( right_composition )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::expr;
  using htparse::fail;
  using htparse::complete_with_error;
  using htparse::complete_with_status;
  using htparse::operator >=;
  using htparse::operator <<;

  context_ptr ctx = context::make();
  message m1(ctx);

  parser p1(m1);

  expr e1([](const message& m) {
      return parse(m.output("", "abc"));
    });

  expr e2([](const message& m) {
      return parse(m.output("", "def"));
    });

  expr e3([](const message& m) {
      return parse(m.output("", "ghi"));
    });

  expr e4 = e1 << e2;
  parser p4 = p1 >= e4;
  message m4 = p4.message();
  BOOST_CHECK( !p4.failed() );
  BOOST_CHECK( !p4.complete() );
  BOOST_CHECK( m4.response_body() == "defabc" );

  expr e5 = e1 << e2 << e3;
  parser p5 = p1 >= e5;
  message m5 = p5.message();
  BOOST_CHECK( !p5.failed() );
  BOOST_CHECK( !p5.complete() );
  BOOST_CHECK( m5.response_body() == "ghidefabc" );

}

BOOST_AUTO_TEST_SUITE_END()
