/**
 * @file   status.hpp
 * @author jedf
 *
 * @brief  The response status of an HTTP transaction.
 */
#pragma once
#ifndef HTPARSE_STATUS_HPP_
#define HTPARSE_STATUS_HPP_
#include <string>

namespace htparse {

using std::string;

/** Some HTTP status codes.
 */
enum class http_code {
  OK = 200,
  BadRequest = 400,
  Unauthorized = 401,
  Forbidden = 403,
  NotFound = 404,
  MethodNotAllowed = 405,
  UnsupportedMediaType = 415,
  InternalServerError = 500
};

/** The response status of an HTTP transaction.
 *
 */
class status
{
private:
  int code_;

  string message_;

public:
  /** Default constructor of #status.
   */
  status():
    code_(0) {}

  /** Constructs an instance of #status.
   *
   * @param[in] code The HTTP status code.
   * @param[in] msg A descriptive message.
   */
  status(int code, const string& msg):
    code_(code),
    message_(msg) {}

  /** Constructs an instance of #status.
   *
   * @param[in] code The HTTP status code.
   * @param[in] msg A descriptive message.
   */
  status(http_code code, const string& msg):
    code_(static_cast<int>(code)),
    message_(msg) {}

  /** Copy constructor of #status.
   *
   * @param[in] other An existing instance of #status.
   */
  status(const status& other):
    code_(other.code_),
    message_(other.message_) {}

  /** Move constructor of #status.
   *
   * @param[in] other An existing instance of #status.
   */
  status(status&& other):
    code_(other.code_),
    message_(std::move(other.message_)) {}

  /** Returns true if the code is zero.
   */
  bool empty() const
  {
    return code_ == 0;
  }

  /** The value of the status code.
   */
  int code() const
  {
    return code_;
  }

  /** The descriptive message.
   */
  const string& message() const
  {
    return message_;
  }

  /** Tests whether the status code  is #BadRequest or greater.
   *
   * @return True if the status code is #BadRequest or greater.
   */
  bool has_error() const
  {
    return code_ >= static_cast<int>(http_code::BadRequest);
  }

  /** Writes the code and descriptive message to the supplied stream.
   *
   * @param[in] stm A stream to write.
   *
   * @return The supplied stream.
   */
  template <class TStm>
  TStm& show(TStm& stm) const
  {
    stm << code_ << ' ' << message_;
    return stm;
  }
};

}
#endif
