/**
 * @file   choice.hpp
 * @author jedf
 *
 * @brief  A filter that tries to apply a sequence of filters in order.
 */
#pragma once
#ifndef HTPARSE_FILTERS_CHOICE_HPP_
#define HTPARSE_FILTERS_CHOICE_HPP_
#include <htparse/filter.hpp>
#include <vector>
#include <utility>

namespace htparse { namespace filters {

/**  A filter that tries to apply a sequence of filters in order.
 */
template <class TMsg>
class choice:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

private:
  std::vector<filter_func> opts_;

public:
  /** Constructs an instance of #choice.
   *
   * @param[in] f_1 The first filter function.
   * @param[in] f_2 The second filter function.
   */
  choice(const filter_func& f_1,
         const filter_func& f_2)
    {
      opts_.reserve(2);
      opts_.push_back(f_1);
      opts_.push_back(f_2);
    }

  /** Constructs an instance of #choice.
   *
   * @param[in] f_1 The first filter function.
   * @param[in] f_2 The second filter function.
   * @param[in] f_2 The third filter function.
   */
  choice(const filter_func& f_1,
         const filter_func& f_2,
         const filter_func& f_3)
    {
      opts_.reserve(3);
      opts_.push_back(f_1);
      opts_.push_back(f_2);
      opts_.push_back(f_3);
    }

  /** Constructs an instance of #choice.
   *
   * @param[in] f_1 The first filter function.
   * @param[in] f_2 The second filter function.
   * @param[in] f_2 The third filter function.
   */
  choice(const std::initializer_list<filter_func>& opts):
    opts_(opts) {}

  /** Copy constructor of #choice.
   *
   * @param[in] other An existing instance of #choice.
   */
  choice(const choice& other):
    opts_(other.opts_) {}

  /** Move constructor of #choice.
   *
   * @param[in] other An existing instance of #choice.
   */
  choice(choice&& other):
    opts_(std::move(other.opts_)) {}

  /** Tries to apply the specified filters in order, until one of them
   * succeeds. Returns the value of the succeeding filter or a failed parser.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator ()(const message_type& msg) const
  {
    if(opts_.empty())
      return fail(msg, set_error);

    for(const auto& fn : opts_)
    {
      if(fn)
      {
        parser_type res = fn(msg);
        if(res.not_failed_or_complete())
          return res;
      }
    }

    return fail(msg, set_error);
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
