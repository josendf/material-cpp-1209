#include "precompiled.h"
#include "test_config.hpp"
#include <htparse/backends/testing.hpp>

BOOST_AUTO_TEST_SUITE( rheader_test )

BOOST_AUTO_TEST_CASE( contruct_from_map )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::expr;
  using htparse::rheader;
  using htparse::operator >=;
  using std::pair;
  using std::make_pair;
  using std::map;
  using std::string;

  context_ptr ctx = context::make();
  message m1(ctx);
  parser p1(m1);

  map <string, string> hdrs;
  hdrs.insert({"Content-Type",     "text/xml"});
  hdrs.insert({"Content-Encoding", "utf-8"});

  parser p2 = p1 >= rheader(hdrs);
  const message& m = p2.message();

  BOOST_REQUIRE( m.has_response_header("Content-Type") );
  BOOST_CHECK(m.response_header("Content-Type").get() == "text/xml");

  BOOST_REQUIRE( m.has_response_header("Content-Encoding") );
  BOOST_CHECK(m.response_header("Content-Encoding").get() == "utf-8");
}

BOOST_AUTO_TEST_CASE( contruct_from_list )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::expr;
  using htparse::rheader;
  using htparse::operator >=;
  using std::pair;
  using std::make_pair;
  using std::map;
  using std::string;

  context_ptr ctx = context::make();
  message m1(ctx);
  parser p1(m1);

  parser p2 = p1 >= rheader {
    {"Content-Type",     "text/xml"},
    {"Content-Encoding", "utf-8"}
    };
  const message& m = p2.message();

  BOOST_REQUIRE( m.has_response_header("Content-Type") );
  BOOST_CHECK(m.response_header("Content-Type").get() == "text/xml");

  BOOST_REQUIRE( m.has_response_header("Content-Encoding") );
  BOOST_CHECK(m.response_header("Content-Encoding").get() == "utf-8");
}

BOOST_AUTO_TEST_CASE( no_cache )
{
  using htparse::context;
  using htparse::context_ptr;
  using htparse::message;
  using htparse::parser;
  using htparse::expr;
  using htparse::no_cache;
  using htparse::operator >=;
  using std::pair;
  using std::make_pair;
  using std::map;
  using std::string;

  context_ptr ctx = context::make();
  message m1(ctx);
  parser p1(m1);

  parser p2 = p1 >= no_cache();
  const message& m = p2.message();

  BOOST_REQUIRE( m.has_response_header("Cache-Control") );
  BOOST_CHECK(m.response_header("Cache-Control").get() == "no-store");

  BOOST_REQUIRE( m.has_response_header("Pragma") );
  BOOST_CHECK(m.response_header("Pragma").get() == "no-cache");
}

BOOST_AUTO_TEST_SUITE_END()
