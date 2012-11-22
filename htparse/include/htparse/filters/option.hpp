/**
 * @file   option.hpp
 * @author jedf
 *
 * @brief  A filter that applies a specified filter, if the filters fails
 * the original message is returned unaltered.
 */
#pragma once
#ifndef HTPARSE_FILTERS_OPTION_HPP_
#define HTPARSE_FILTERS_OPTION_HPP_
#include <htparse/filter.hpp>
#include <utility>

namespace htparse { namespace filters {

/** A filter that applies a specified filter, if the filter fails
 * the original message is returned unaltered.
 */
template <class TMsg>
class option:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

private:
  filter_func func_;

public:
  /** Constructs an instance of #option.
   *
   * @param[in] func The filter function.
   */
  option(const filter_func& func):
    func_(func) {}

  /** Copy constructor of #option.
   *
   * @param[in] other An existing instance of #option.
   */
  option(const option& other):
    func_(other.func_) {}

  /** Move constructor of #option.
   *
   * @param[in] other An existing instance of #basic_choice.
   */
  option(option&& other):
    func_(std::move(other.func_)) {}

  /** Tries to apply the specified filter, if the filter fails
   * the original message is returned unaltered.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator ()(const message_type& msg) const
  {
    if(func_)
    {
      parser_type res = func_(msg);
      if(!res.complete_or_failed())
        return res;
    }
    return parse(msg);
  }
};

}}
#endif
