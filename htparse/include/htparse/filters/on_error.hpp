/**
 * @file   on_error.hpp
 * @author jedf
 *
 * @brief  Filters to deal with error conditions.
 */
#pragma once
#ifndef HTPARSE_FILTERS_ON_ERROR_HPP_
#define HTPARSE_FILTERS_ON_ERROR_HPP_
#include <htparse/filter.hpp>
#include <utility>

namespace htparse { namespace filters {

/** A filter that invokes another filter when
 * the specified expresion fails, otherwise the original
 * message is returned unaltered.
 */
template <class TMsg>
class on_error:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

private:
  filter_func expr_;
  filter_func handler_;

public:
  /** Constructs an instance of #on_error.
   *
   * @param[in] exp The expression to evaluate.
   * @param[in] handler The failure handler.
   */
  on_error(const filter_func& exp, const filter_func& handler):
    expr_(exp),
    handler_(handler) {}

  /** Copy constructor of #on_error.
   *
   * @param[in] other An existing instance of #on_error.
   */
  on_error(const on_error& other):
    expr_(other.expr_),
    handler_(other.handler_) {}

  /** Move constructor of #on_error.
   *
   * @param[in] other An existing instance of #basic_choice.
   */
  on_error(on_error&& other):
    expr_(std::move(other.expr_)),
    handler_(std::move(other.handler_)) {}

  /** If the specified parser failed returns a complete parser message
   * preventing any further parsing, otherwise the original message is
   * returned unaltered.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator ()(const message_type& msg) const
  {
    if(expr_)
    {
      parser_type res = expr_(msg);
      const message_type& rmsg = res.message();
      if(handler_ && (res.failed() || rmsg.status().has_error()))
        return handler_(rmsg);
    }
    return parse(msg);
  }
};

/** A filter that returns a complete parser, preventing any further
 * parsing, when the specified expression fails, otherwise
 * the original message is returned unaltered.
 */
template <class TMsg>
class on_error_complete:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

private:
  filter_func expr_;
  status_type status_;

public:
  /** Constructs an instance of #on_error_complete.
   *
   * @param[in] exp The expression to evaluate.
   */
  on_error_complete(const filter_func& exp):
    expr_(exp) {}

  /** Constructs an instance of #on_error_complete.
   *
   * @param[in] exp The expression to evaluate.
   * @param[in] status The final status.
   */
  on_error_complete(const filter_func& exp, const status_type& status):
    expr_(exp),
    status_(status) {}

  /** Copy constructor of #on_error_complete.
   *
   * @param[in] other An existing instance of #on_error.
   */
  on_error_complete(const on_error_complete& other):
    expr_(other.expr_),
    status_(other.status_) {}

  /** Move constructor of #on_error_complete.
   *
   * @param[in] other An existing instance of #basic_choice.
   */
  on_error_complete(on_error_complete&& other):
    expr_(std::move(other.expr_)),
    status_(std::move(other.status_)) {}

  /** If the specified parser failed returns a complete parser message
   * preventing any further parsing, otherwise the original message is
   * returned unaltered.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator ()(const message_type& msg) const
  {
    if(expr_)
    {
      parser_type res = expr_(msg);
      const message_type& rmsg = res.message();
      if(!res.complete() && (res.failed() || rmsg.status().has_error()))
      {
        if(!status_.empty())
          return complete_with_error(rmsg, status_);
        return complete(rmsg);
      }
      return res;
    }
    return parse(msg);
  }
};

}}
#endif
