/**
 * @file   helpers.hpp
 * @author jedf
 *
 * @brief  Utilities.
 */
#pragma once
#ifndef HTPARSE_HELPERS_HPP_
#define HTPARSE_HELPERS_HPP_
#include <string>
#include <utility>
#include <cstdlib>

namespace htparse { namespace helpers {
using std::string;
using std::pair;

/** Parses the Host HTTP header.
 */
template <class TStr>
pair<TStr, int> host_name_and_port(const TStr& host_header,
                                   const int default_port)
{
  typedef TStr string_type;
  typedef typename string_type::const_reverse_iterator rit_type;

  std::size_t pos = host_header.length();
  if(pos < 1)
    return std::make_pair<TStr, int>(string_type(), 0);

  for (rit_type i = host_header.rbegin(), e = host_header.rend();
       i != e; ++i, --pos)
  {
    if(*i == ':')
    {
      const string_type hst = (pos > 0) ?
        host_header.substr(0, pos - 1) : string_type();
      const int prt = std::atoi(host_header.c_str() + pos);
      return std::make_pair(hst, prt);
    }
  }

  return std::make_pair(host_header, default_port);
}


}}
#endif
