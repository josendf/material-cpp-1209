#include "precompiled.h"
#include "test_config.hpp"
#include <htparse/backends/testing.hpp>
#include <htparse/auth/mac.hpp>

BOOST_AUTO_TEST_SUITE( hmac_test )

BOOST_AUTO_TEST_CASE( valid_header )
{
  using htparse::auth::mac::header_parser;
  using htparse::auth::mac::header;
  using boost::optional;
  using std::string;
  header_parser par;
  string s;
  optional<header> ret;

  s = R"(MAC id="h480djs93hd8",
             ts="1336363200",
             nonce="dj83hs9s",
             ext="abc",
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_REQUIRE( ret );
  {
    const header& h = ret.get();
    BOOST_REQUIRE( ret );
    BOOST_CHECK(h.id_ == "h480djs93hd8");
    BOOST_CHECK(h.ts_ == 1336363200);
    BOOST_CHECK(h.nonce_ == "dj83hs9s");
    BOOST_CHECK(h.ext_ == "abc");
    BOOST_CHECK(h.mac_ == "6T3zZzy2Emppni6bzL7kdRxUWL4=");
  }

  s = R"(MAC  id = "h480djs93hd8",
              ts = "1336363200" ,
              nonce ="dj83hs9s",
              ext="",
              mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_REQUIRE( ret );
  {
    const header& h = ret.get();
    BOOST_REQUIRE( ret );
    BOOST_CHECK(h.id_ == "h480djs93hd8");
    BOOST_CHECK(h.ts_ == 1336363200);
    BOOST_CHECK(h.nonce_ == "dj83hs9s");
    BOOST_CHECK(h.ext_ == "");
    BOOST_CHECK(h.mac_ == "6T3zZzy2Emppni6bzL7kdRxUWL4=");
  }

  s = R"(MAC id="h480djs93hd8" ,
             ts = 1336363200 ,
             nonce="dj83hs9s",
             ext=abc ,
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_REQUIRE( ret );
  {
    const header& h = ret.get();
    BOOST_REQUIRE( ret );
    BOOST_CHECK(h.id_ == "h480djs93hd8");
    BOOST_CHECK(h.ts_ == 1336363200);
    BOOST_CHECK(h.nonce_ == "dj83hs9s");
    BOOST_CHECK(h.ext_ == "abc");
    BOOST_CHECK(h.mac_ == "6T3zZzy2Emppni6bzL7kdRxUWL4=");
  }

  s = R"(MAC id="h480djs93hd8" ,
             ts = 1336363200 ,
             nonce="dj83hs9s",
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_REQUIRE( ret );
  {
    const header& h = ret.get();
    BOOST_REQUIRE( ret );
    BOOST_CHECK(h.id_ == "h480djs93hd8");
    BOOST_CHECK(h.ts_ == 1336363200);
    BOOST_CHECK(h.nonce_ == "dj83hs9s");
    BOOST_CHECK(h.ext_.empty() );
    BOOST_CHECK(h.mac_ == "6T3zZzy2Emppni6bzL7kdRxUWL4=");
  }

  s = R"(MAC ts = 1336363200,
             nonce="dj83hs9s",
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=",
             id="h480djs93hd8")";
  ret = par.parse(s);
  BOOST_REQUIRE( ret );
  {
    const header& h = ret.get();
    BOOST_REQUIRE( ret );
    BOOST_CHECK(h.id_ == "h480djs93hd8");
    BOOST_CHECK(h.ts_ == 1336363200);
    BOOST_CHECK(h.nonce_ == "dj83hs9s");
    BOOST_CHECK(h.ext_.empty() );
    BOOST_CHECK(h.mac_ == "6T3zZzy2Emppni6bzL7kdRxUWL4=");
  }
}

BOOST_AUTO_TEST_CASE( invalid_header )
{
  using htparse::auth::mac::header_parser;
  using htparse::auth::mac::header;
  using boost::optional;
  using std::string;
  header_parser par;
  string s;
  optional<header> ret;

  s = R"(MAC id="",
             ts=1336363200,
             nonce="dj83hs9s",
             ext="abc",
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_CHECK( !ret );

  s = R"(MAC ts=1336363200,
             nonce="dj83hs9s",
             ext="abc",
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_CHECK( !ret );

  s = R"(MAC id="h480djs93hd8",
             ts=0,
             nonce="dj83hs9s",
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_CHECK( !ret );

  s = R"(MAC id="h480djs93hd8",
             ts=,
             nonce="dj83hs9s",
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_CHECK( !ret );

  s = R"(MAC id="h480djs93hd8",
             nonce="dj83hs9s",
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_CHECK( !ret );

  s = R"(MAC id="h480djs93hd8",
             ts=1336363200,
             nonce="",
             mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";
  ret = par.parse(s);
  BOOST_CHECK( !ret );

  s = R"(MAC id="h480djs93hd8",
             ts=1336363200,
             nonce="dj83hs9s",
             mac="")";
  ret = par.parse(s);
  BOOST_CHECK( !ret );
}

BOOST_AUTO_TEST_CASE( norm_request )
{
  using htparse::auth::mac::norm_request;
  using std::string;

  const string expected(
    "123456\n"
    "xyz123\n"
    "POST\n"
    "/resources/one/two/three\n"
    "example.com\n"
    "80\n"
    "a,b,c\n"
    );

  norm_request rs;

  rs.ts()     = 123456;
  rs.nonce()  = "xyz123";
  rs.method() = "POST";
  rs.uri()    = "/resources/one/two/three";
  rs.host()   = "example.com";
  rs.port()   = 80;
  rs.ext()    = "a,b,c";
  {
    string ret = rs.normalize();
    BOOST_CHECK( ret == expected );
  }

  rs.ts()     = 123456;
  rs.nonce()  = "xyz123";
  rs.method() = "PoSt";
  rs.uri()    = "/resources/one/two/three";
  rs.host()   = "ExaMple.Com";
  rs.port()   = 80;
  rs.ext()    = "a,b,c";
  {
    string ret = rs.normalize();
    BOOST_CHECK( ret == expected );
  }
}

BOOST_AUTO_TEST_CASE( mac_auth_filter )
{
  using std::string;
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::request;
  using htparse::parser;
  using htparse::complete_ok;
  using htparse::mac_auth;
  typedef htparse::auth::mac::header mac_header;
  typedef htparse::auth::mac::norm_request norm_request;

  context_ptr ctx = context::make();
  request& req = ctx->request();
  message m1(ctx);
  parser p1(m1);

  const string auth =
    R"(MAC id="h480djs93hd8",
           ts="1336363200",
           nonce="dj83hs9s",
           ext="abc",
           mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";


  req.method() = "POST";
  req.host_name() = "example.com";
  req.port() = 80;
  req.path() = "/a/b/c";
  req.set_header("Authorization", auth);

  {
    parser p = p1 >=
      mac_auth([](const message& m,
                  const mac_header& h,
                  const norm_request& r) {

                 BOOST_CHECK( h.ts_ == 1336363200);
                 BOOST_CHECK( h.mac_ == "6T3zZzy2Emppni6bzL7kdRxUWL4=");
                 BOOST_CHECK( r.method() == "POST" );
                 BOOST_CHECK( r.host() == "example.com" );
                 BOOST_CHECK( r.port() == 80 );
                 BOOST_CHECK( r.uri() == "/a/b/c" );

                 return complete_ok(m, "text/plain", "\n");
               });

    BOOST_CHECK( !p.failed() );
    BOOST_CHECK( p.complete() );
    BOOST_CHECK( p.message().has_response_content_type("text/plain") );
  }
}

BOOST_AUTO_TEST_CASE( x_auth_headers )
{
  using std::string;
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::request;
  using htparse::parser;
  using htparse::complete_ok;
  using htparse::mac_auth;
  typedef htparse::auth::mac::header mac_header;
  typedef htparse::auth::mac::norm_request norm_request;

  context_ptr ctx = context::make();
  request& req = ctx->request();
  message m1(ctx);
  parser p1(m1);

  const string auth =
    R"(MAC id="h480djs93hd8",
           ts="1336363200",
           nonce="dj83hs9s",
           ext="abc",
           mac="6T3zZzy2Emppni6bzL7kdRxUWL4=")";


  req.method() = "POST";
  req.host_name() = "example.com";
  req.port() = 80;
  req.path() = "/a/b/c";
  req.set_header("Authorization", auth);

  req.set_header("X-Auth-Method", "GET");
  req.set_header("X-Auth-Host", "secure.com:8080");
  req.set_header("X-Auth-URL", "/x/y/z");

  {
    parser p = p1 >=
      mac_auth([](const message& m,
                  const mac_header& h,
                  const norm_request& r) {

                 BOOST_CHECK( h.ts_ == 1336363200);
                 BOOST_CHECK( h.mac_ == "6T3zZzy2Emppni6bzL7kdRxUWL4=");
                 BOOST_CHECK( r.method() == "GET" );
                 BOOST_CHECK( r.host() == "secure.com" );
                 BOOST_CHECK( r.port() == 8080 );
                 BOOST_CHECK( r.uri() == "/x/y/z" );

                 return complete_ok(m, "text/plain", "\n");
               });

    BOOST_CHECK( !p.failed() );
    BOOST_CHECK( p.complete() );
    BOOST_CHECK( p.message().has_response_content_type("text/plain") );
  }
}

BOOST_AUTO_TEST_SUITE_END()
