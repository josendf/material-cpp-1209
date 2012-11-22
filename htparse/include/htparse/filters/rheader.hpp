/**
 * @file   rheader.hpp
 * @author jedf
 *
 * @brief  A filter that appends response headers to the message.
 */

#pragma once
#ifndef HTPARSE_FILTERS_RHEADER_HPP_
#define HTPARSE_FILTERS_RHEADER_HPP_
#include <htparse/filter.hpp>
#include <string>
#include <map>
#include <vector>

namespace htparse { namespace filters {

using std::string;
using std::map;
using std::vector;
using std::initializer_list;
using std::pair;

/** A filter that appends response headers to the message.
 */
template <class TMsg>
class rheader:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

private:
  typedef map<string, string> header_type;

  header_type header_;

public:
  rheader(const header_type& header):
    header_(header) {}

  rheader(const initializer_list<pair<const char*, const char*> >&  ini)
  {
    using std::make_pair;
    for(const auto& p : ini)
    {
      if(p.first && (*p.first) && p.second)
      {
        header_.insert(make_pair<string, string>(p.first, p.second));
      }
    }
  }

  rheader(const rheader& other):
    header_(other.header_) {}

  rheader(rheader&& other):
    header_(std::move(other.header_)) {}

  parser_type operator()(const message_type& msg) const
  {
    if(header_.empty())
      return parse(msg);
    return parse(msg.set_response_headers(header_));
  }
};

/** A filter that appends Cache-Control and Pragma
 * response headers to the message.
 */
template <class TMsg>
class no_cache:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

private:
  typedef vector<pair<string, string> > header_type;

  header_type header_;

public:
  no_cache()
  {
    header_.push_back(header_type::value_type("Cache-Control","no-store"));
    header_.push_back(header_type::value_type("Pragma","no-cache"));
  }

  no_cache(const no_cache& other):
    header_(other.header_) {}

  no_cache(no_cache&& other):
    header_(std::move(other.header_)) {}

  parser_type operator()(const message_type& msg) const
  {
    return parse(msg.set_response_headers(header_));
  }
};

}}
#endif
