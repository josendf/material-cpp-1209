#include "precompiled.h"
#include "test_config.hpp"
#include <htparse/backends/testing.hpp>

BOOST_AUTO_TEST_SUITE( path_test )

BOOST_AUTO_TEST_CASE( match )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::request;
  using htparse::parser;
  using htparse::expr;
  using htparse::fail;
  using htparse::path;

  context_ptr ctx = context::make();
  request& req = ctx->request();
  message m1(ctx);
  parser p1(m1);

  req.path() = "/a";
  {
     expr e = path("^/a$");
     parser p = p1 >= e;
     message m = p.message();

     BOOST_CHECK( !p.failed() );
     BOOST_CHECK( !p.complete() );
  }

  req.path() = "/a/b/c";
  {
     expr e = path("^/a/b/c$");
     parser p = p1 >= e;
     message m = p.message();

     BOOST_CHECK( !p.failed() );
     BOOST_CHECK( !p.complete() );
  }
}

BOOST_AUTO_TEST_CASE( no_match )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::request;
  using htparse::parser;
  using htparse::expr;
  using htparse::fail;
  using htparse::path;

  context_ptr ctx = context::make();
  request& req = ctx->request();
  message m1(ctx);
  parser p1(m1);

  req.path() = "";
  {
     expr e = path("^/a/b/c$");
     parser p = p1 >= e;
     message m = p.message();

     BOOST_CHECK( p.failed() );
     BOOST_CHECK( !p.complete() );
  }

  req.path() = "/a";
  {
     expr e = path("^/a/b/c$");
     parser p = p1 >= e;
     message m = p.message();

     BOOST_CHECK( p.failed() );
     BOOST_CHECK( !p.complete() );
  }
}

BOOST_AUTO_TEST_CASE( parts )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::request;
  using htparse::parser;
  using htparse::expr;
  using htparse::fail;
  using htparse::path;
  typedef htparse::path::part part;

  context_ptr ctx = context::make();
  request& req = ctx->request();
  message m1(ctx);
  parser p1(m1);

  req.path() = "/a/b/c";
  {
    expr e = path("^/a", "/(\\w+)", "/(\\w+)$",
                  [](const message& msg,
                     const part& p0,
                     const part& p1,
                     const part& p2)
                  {
                    BOOST_REQUIRE( p0 );
                    BOOST_CHECK( p0.get() == "/a/b/c"  );
                    BOOST_REQUIRE( p1 );
                    BOOST_CHECK( p1.get() == "b"  );
                    BOOST_REQUIRE( p2 );
                    BOOST_CHECK( p2.get() == "c"  );
                    return parse(msg);
                  });

     parser p = p1 >= e;
     message m = p.message();
     BOOST_CHECK( !p.failed() );
     BOOST_CHECK( !p.complete() );
  }

  req.path() = "/a/b";
  {
    expr e = path("^/a", "/(\\w+)", "/(\\w+)$",
                  [](const message& msg,
                     const part& p0,
                     const part& p1,
                     const part& p2)
                  {
                    BOOST_CHECK( false );
                    return parse(msg);
                  });

     parser p = p1 >= e;
     message m = p.message();
     BOOST_CHECK( p.failed() );
     BOOST_CHECK( !p.complete() );
  }

  req.path() = "/a";
  {
    expr e = path("^/a", "/(\\w+)", "/(\\w+)$",
                  [](const message& msg,
                     const part& p0,
                     const part& p1,
                     const part& p2)
                  {
                    BOOST_CHECK( false );
                    return parse(msg);
                  });

     parser p = p1 >= e;
     message m = p.message();
     BOOST_CHECK( p.failed() );
     BOOST_CHECK( !p.complete() );
  }
}

BOOST_AUTO_TEST_SUITE_END()
