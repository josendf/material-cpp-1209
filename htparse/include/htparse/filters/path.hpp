/**
 * @file   path.hpp
 * @author jedf
 *
 * @brief  A filter that checks the request path.
 */
#pragma once
#ifndef HTPARSE_FILTERS_PATH_HPP_
#define HTPARSE_FILTERS_PATH_HPP_
#include <htparse/filter.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/regex.hpp>

namespace htparse { namespace filters {

using std::string;
using boost::function;
using boost::optional;
using boost::regex;
using boost::regex_match;
using boost::smatch;
using boost::regex_constants::normal;
using boost::regex_constants::icase;
using boost::regex_constants::no_except;
using boost::regex_constants::match_single_line;

/** A filter that checks the request path.
 */
template <class TMsg>
class path:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

  typedef optional<string> part;

  typedef function<parser_type (const message_type&,
                                const part&)> func_1;

  typedef function<parser_type (const message_type&,
                                const part&,
                                const part&)> func_2;

  typedef function<parser_type (const message_type&,
                                const part&,
                                const part&,
                                const part&)> func_3;

  typedef function<parser_type (const message_type&,
                                const part&,
                                const part&,
                                const part&,
                                const part&)> func_4;

private:
  regex pattern_;

  func_1 func_1_;
  func_2 func_2_;
  func_3 func_3_;
  func_4 func_4_;

public:
  path(const path& other):
    pattern_(other.pattern_),
    func_1_(other.func_1_),
    func_2_(other.func_2_),
    func_3_(other.func_3_) {}

  path(path&& other):
    pattern_(std::move(other.pattern_)),
    func_1_(std::move(other.func_1_)),
    func_2_(std::move(other.func_2_)),
    func_3_(std::move(other.func_3_)),
    func_4_(std::move(other.func_4_)) {}

  path(const string& p):
    pattern_(p, normal | icase | no_except) {}

  path(const string& p,
       const func_1& func):
    pattern_(p, normal | icase | no_except),
    func_1_(func) {}

  path(const string& p_1,
       const string& p_2,
       const func_2& func):
    pattern_(p_1 + p_2, normal | icase | no_except),
    func_2_(func) {}

  path(const string& p_1,
       const string& p_2,
       const string& p_3,
       const func_3& func):
    pattern_(p_1 + p_2 + p_3, normal | icase | no_except),
    func_3_(func) {}

  path(const string& p_1,
       const string& p_2,
       const string& p_3,
       const string& p_4,
       const func_4& func):
    pattern_(p_1 + p_2 + p_3 + p_4, normal | icase | no_except),
    func_3_(func) {}

  parser_type operator()(const message_type& msg) const
  {
    // Important: copy path to local
    const string path(msg.request().path());
      
    smatch what;
    if(!regex_match(path, what, pattern_, match_single_line))
      return fail(msg, set_error);

    part v1;
    if(what.size() > 0)
      v1 = what[0];

    if(func_1_)
      return func_1_(msg, v1);

    part v2;
    if(what.size() > 1)
      v2 = what[1];

    if(func_2_)
      return func_2_(msg, v1, v2);

    part v3;
    if(what.size() > 2)
      v3 = what[2];

    if(func_3_)
      return func_3_(msg, v1, v2, v3);

    part v4;
    if(what.size() > 3)
      v4 = what[3];

    if(func_4_)
      return func_4_(msg, v1, v2, v3, v4);

    return parse(msg);
  }

private:
  static
  status_type set_error()
  {
    return status_type(404, "Not Found");
  }

};

}}
#endif
