/**
 * @file   filter.hpp
 * @author jedf
 *
 * @brief  Functions that takes a message and returns a parser.
 */
#pragma once
#ifndef HTPARSE_FILTER_HPP_
#define HTPARSE_FILTER_HPP_
#include <ext/boost/function.hpp>

namespace htparse {

using boost::function;

/** A function that takes a message and returns a parser.
 */
template <class TMsg>
struct basic_filter
{
  typedef TMsg message_type;
  typedef basic_parser<TMsg> parser_type;
  typedef function<parser_type (const message_type&)> filter_func;
  typedef typename message_type::status_type status_type;
};

/** Holds a function that takes a message and returns a parser.
 */
template <class TMsg>
class basic_expr:
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
  basic_expr() {}

  template<class T>
  basic_expr(const T& func):
    func_(func) {}

  basic_expr(const basic_expr& other):
    func_(other.func_) {}

  basic_expr(basic_expr&& other):
    func_(std::move(other.func_)) {}

  parser_type operator ()(const message_type& msg) const
  {
    if(func_)
      return func_(msg);
    return parse(msg);
  }
};


/** Conditionally apply a filter function to a message.
 *
 * @param[in] p A parser.
 * @param[in] f A function that takes a Message and returns a Parser.
 * @return If the specified parser has failed, or it is complete, returns
 * the specified parser. Otherwise returns the result of applying the
 * specified function to the message.
 */
template <class TMsg, class TFunc>
inline
basic_parser<TMsg>
operator >=(const basic_parser<TMsg>& p, const TFunc& f)
{
  if(p.failed_or_complete())
    return p;
  return f(p.message());
}

/** Left-to-right composition of filters.
 *
 * @param[in] f The left filter function.
 * @param[in] g The right filter function.
 *
 * @return The left-to-right composition of f and g.
 */
template <class TF, class TG>
inline
basic_expr<typename TF::message_type>
operator >>(const TF& f, const TG& g)
{
  typedef typename TF::message_type message_type;
  return [=](const message_type& msg){ return f(msg) >= g; };
}

/** Right-to-left composition of filters.
 *
 * @param[in] f The left filter function.
 * @param[in] g The right filter function.
 *
 * @return The right-to-left composition of f and g.
 */
template <class TF, class TG>
inline
basic_expr<typename TF::message_type>
operator <<(const TF& f, const TG& g)
{
  typedef typename TF::message_type message_type;
  return [=](const message_type& msg){ return g(msg) >= f; };
}

}
#endif
