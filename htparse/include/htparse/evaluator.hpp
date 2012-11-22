/**
 * @file   evaluator.hpp
 * @author jedf
 *
 * @brief  Evaluates a filter expression.
 */
#pragma once
#ifndef HTPARSE_EVALUATOR_HPP_
#define HTPARSE_EVALUATOR_HPP_
#include <htparse/parser.hpp>
#include <htparse/filter.hpp>
#include <ext/boost/shared_ptr.hpp>
#include <ext/boost/make_shared.hpp>

namespace htparse {

using boost::shared_ptr;
using boost::make_shared;

/** Evaluates a filter expression.
 */
template <class TCtx, class TMsg, class TPar>
class basic_evaluator
{
public:
  typedef TCtx context_type;
  typedef typename TCtx::response_type response_type;
  typedef shared_ptr<TCtx> context_ptr;
  typedef TMsg message_type;
  typedef typename TMsg::status_type status_type;
  typedef TPar parser_type;

private:
  context_ptr context_;

public:
  basic_evaluator(const context_ptr& ctx):
    context_(ctx) {}

  basic_evaluator(const basic_evaluator& other):
    context_(other.context_) {}

  basic_evaluator(basic_evaluator&& other):
    context_(std::move(other.context_)) {}

  template <class TExp>
  parser_type operator()(const TExp& exp)
  {
    using htparse::http_code;
    using htparse::parse;

    message_type left(context_);
    parser_type par = parse(left) >= exp;
    message_type right = par.message();

    string phrase;
    const int code = resolve_status(par, right, phrase);

    response_type& resp = context_->response();
    if(par.failed())
    {
      resp.make_error_response(code, phrase);
      return par;
    }

    resp.status(code, phrase);
    resp.set_headers(right.response_header());
    resp.out(right.response_content_type(),
             right.response_body());

    return par;
  }

private:
  static
  int resolve_status(const parser_type& par,
                     const message_type& msg,
                     string& phrase)
  {
    status_type status(msg.status());
    int code = 0;
    if(status.empty())
    {
      if(par.failed())
      {
        code = static_cast<int>(http_code::InternalServerError);
        phrase = "Request parsing failed.";
      }
      else
      {
        code = static_cast<int>(http_code::OK);
        phrase = "OK";
      }
    }
    else
    {
      code = status.code();
      phrase = status.message();
    }
    return code;
  }
};

template <class TCtx, class TMsg, class TPar, class TExpr>
inline
TPar evaluate(const shared_ptr<TCtx>& ctx, const TExpr& expr)
{
  basic_evaluator<TCtx, TMsg, TPar> ev(ctx);
  return ev(expr);
}

}
#endif
