/**
 * @file   context.hpp
 * @author jedf
 * @date   Sat Jun 30 21:42:14 2012
 *
 * @brief  The context of an HTTP transaction.
 */
#pragma once
#ifndef HTPARSE_BACKENDS_TESTING_CONTEXT_HPP_
#define HTPARSE_BACKENDS_TESTING_CONTEXT_HPP_
#include <utility>
#include <ext/boost/shared_ptr.hpp>
#include <ext/boost/make_shared.hpp>

namespace htparse { namespace testing {

using boost::shared_ptr;
using boost::make_shared;

/** The context of an HTTP transaction.
 */
template <class TRawCtx, class TReq, class TResp>
class basic_context
{
public:
  /** The request object type.
   */
  typedef TReq request_type;

  /** The response object type.
   */
  typedef TResp response_type;

  typedef TRawCtx raw_context_type;

  typedef raw_context_type* raw_context_ptr;

  typedef shared_ptr<basic_context> context_ptr;

private:
  raw_context_ptr raw_context_;

  request_type request_;

  response_type response_;

public:
  /** Constructs an instance of #basic_context.
   */
  basic_context():
    raw_context_(0) {}

  /** Constructs an instance of #basic_context.
   */
  basic_context(raw_context_ptr rctx):
    raw_context_(rctx) {}

  /** Copy constructor of #basic_context.
   *
   * @param[in] other An existing instance of #basic_context.
   */
  basic_context(const basic_context& other):
    raw_context_(other.raw_context_),
    request_(other.request_),
    response_(other.response_) {}

  /** Move constructor of #basic_context.
   *
   * @param[in] other An existing instance of #basic_context.
   */
  basic_context(basic_context&& other):
    raw_context_(other.raw_context_),
    request_(std::move(other.request_)),
    response_(std::move(other.response_)) {}

  static
  inline context_ptr make()
  {
    return make_shared<basic_context>();
  }

  static
  inline context_ptr make(raw_context_ptr rctx)
  {
    return make_shared<basic_context>(rctx);
  }

  raw_context_ptr raw_context()
  {
    return raw_context_;
  }

  const raw_context_ptr raw_context() const
  {
    return raw_context_;
  }

  /** The HTTP request.
   *
   * @return The request object.
   */
  request_type& request()
  {
    return request_;
  }

  /** The HTTP request.
   *
   * @return The request object.
   */
  const request_type& request() const
  {
    return request_;
  }

  /** The HTTP response.
   *
   * @return The response object.
   */
  response_type& response()
  {
    return response_;
  }

  /** The HTTP response.
   *
   * @return The response object.
   */
  const response_type& response() const
  {
    return response_;
  }
};

}}
#endif
