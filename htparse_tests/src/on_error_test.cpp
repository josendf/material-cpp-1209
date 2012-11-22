#include "precompiled.h"
#include "test_config.hpp"
#include <htparse/backends/testing.hpp>

BOOST_AUTO_TEST_SUITE( on_error_test )

BOOST_AUTO_TEST_CASE( on_error_complete )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::request;
  using htparse::parser;
  using htparse::expr;
  using htparse::fail;
  using htparse::complete_ok;
  using htparse::path;
  using htparse::method;
  using htparse::choice;
  using htparse::on_error_complete;
  using htparse::operator >=;
  using htparse::operator >>;

  context_ptr ctx = context::make();
  request& req = ctx->request();
  message m1(ctx);
  parser p1(m1);

  expr e_ok([](const message& m) {
      return complete_ok(m, "text/plain", "abc");
    });

  req.method() = "POST";
  req.path() = "/a/b/c";
  {
     expr e1 = path("^/a/b/c$") >> method("POST") >> e_ok;
     expr e2 = path("^/x/y/z$") >> method("GET") >> e_ok;
     parser p = p1 >= choice(e1, e2);
     message m = p.message();

     BOOST_CHECK( !p.failed() );
     BOOST_CHECK( p.complete() );
     BOOST_CHECK( m.has_response_content_type("text/plain") );
  }

  req.method() = "GET";
  req.path() = "/a/b/c";
  {
     expr e1 = path("^/a/b/c$") >> method("POST") >> e_ok;
     expr e2 = path("^/x/y/z$") >> method("GET") >> e_ok;
     parser p = p1 >= choice(e1, e2);
     message m = p.message();

     BOOST_CHECK( p.failed() );
     BOOST_CHECK( !p.complete() );
     BOOST_CHECK( m.status().code() == 404 );
  }

  req.method() = "GET";
  req.path() = "/a/b/c";
  {
     expr e1 = path("^/a/b/c$") >> on_error_complete(method("POST") >> e_ok);
     expr e2 = path("^/x/y/z$") >> method("GET") >> e_ok;
     parser p = p1 >= choice(e1, e2);
     message m = p.message();

     BOOST_CHECK( p.complete() );
     BOOST_CHECK( m.status().code() == 405 );
  }

}

BOOST_AUTO_TEST_SUITE_END()
