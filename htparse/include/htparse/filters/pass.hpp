/**
 * @file   pass.hpp
 * @author jedf
 *
 * @brief  A filter that returns the message unaltered.
 */
#pragma once
#ifndef HTPARSE_FILTERS_PASS_HPP_
#define HTPARSE_FILTERS_PASS_HPP_
#include <htparse/filter.hpp>
#include <string>

namespace htparse { namespace filters {

using std::string;

/** A filter that returns the message unaltered.
 */
template <class TMsg>
class pass:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;

public:
  parser_type operator()(const message_type& msg) const
  {
    return parse(msg);
  }
};

}}
#endif
