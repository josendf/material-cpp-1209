/**
 * @file   bearer.hpp
 * @author jedf
 * @date   Wed Aug 22 16:25:54 2012
 *
 * @brief  Bearer Access Authentication helpers.
 *         As IETF Draft
 *         http://tools.ietf.org/html/draft-ietf-oauth-v2-bearer-23
 */

#pragma once
#ifndef HTPARSE_AUTH_BEARER_HPP_
#define HTPARSE_AUTH_BEARER_HPP_
#include <string>
#include <ext/boost/spirit.hpp>
#include <ext/boost/optional.hpp>

namespace htparse { namespace auth { namespace bearer {
using std::string;
using boost::optional;

namespace detail {

/** A simple Bearer Authorization header parser.
 */
template <class TIter>
struct basic_token_parser:
    boost::spirit::qi::grammar<TIter,
                               string(),
                               boost::spirit::ascii::space_type>
{
  typedef TIter iter_type;
  typedef string result_type;
  typedef boost::spirit::ascii::space_type space_type;

  boost::spirit::qi::rule<iter_type, result_type(), space_type> start_;
  boost::spirit::qi::rule<iter_type, string(), space_type> token_;

  basic_token_parser(const int max_token_length):
    basic_token_parser::base_type(start_)
  {
     using boost::spirit::qi::lit;
     using boost::spirit::qi::lexeme;
     using boost::spirit::qi::ascii::char_;
     using boost::spirit::qi::repeat;

     start_ = lit("Bearer") >> token_;

     token_ = repeat(1, max_token_length)[( char_("a-zA-Z0-9")
                                          | char_('-')
                                          | char_('.')
                                          | char_('_')
                                          | char_('~')
                                          | char_('+')
                                          | char_('/')
                                          | char_('='))];
  }
};

} // namespace detail

/** Parses the Bearer Authorization request header.
 *
 * Authorization: Bearer mF_9.B5f-4.1JqM
 *
 */
template <class TStr>
class basic_header_parser
{
public:
  typedef TStr string_type;
  typedef typename string_type::const_iterator iter_type;
  typedef detail::basic_token_parser<iter_type> parser_type;

private:
  parser_type parser_;

public:
  basic_header_parser():
    parser_(1024) {}

  optional<string> parse(const string_type& str) const
  {
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::ascii::space;
    typedef typename parser_type::result_type result_type;

    result_type res;
    iter_type beg = std::begin(str);
    const bool ret = phrase_parse(beg, std::end(str), parser_, space, res);
    if(!ret)
      return optional<string>();
    return optional<string>(res);
  }
};
typedef basic_header_parser<string> header_parser;


}}}
#endif
