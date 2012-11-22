/**
 * @file   parser.hpp
 * @author jedf
 *
 * @brief  A message parser.
 */
#pragma once
#ifndef HTPARSE_PARSER_HPP_
#define HTPARSE_PARSER_HPP_
#include <utility>

namespace htparse {

/** Whether a parser has failed.
 */
enum class parser_failed
{
  /// Not run or suceeded.
  unknown,
  /// Failed to fulfill expectations.
  failed
};

/** Whether a parser is complete.
 */
enum class parser_complete
{
  /// Not run or can continue.
  unknown,
  /// No further parsing should be done.
  complete
};

/** A message parser.
 */
template <class TMsg>
class basic_parser
{
public:
  typedef TMsg message_type;

private:
  message_type msg_;
  parser_failed failed_;
  parser_complete complete_;

public:
  /** Constructs an instance of #basic_parser.
   *
   * @param[in] msg The message beign parsed.
   */
  basic_parser(const message_type& msg):
    msg_(msg),
    failed_(parser_failed::unknown),
    complete_(parser_complete::unknown) {}

  /** Constructs an instance of #basic_parser.
   *
   * @param[in] msg The message beign parsed.
   * @param[in] state The state of the parser.
   */
  basic_parser(const message_type& msg,
               parser_failed state):
    msg_(msg),
    failed_(state),
    complete_(parser_complete::unknown) {}

  /** Constructs an instance of #basic_parser.
   *
   * @param[in] msg The message beign parsed.
   * @param[in] state The state of the parser.
   * @param[in] comp Whether the parsing is complete.
   */
  basic_parser(const message_type& msg,
               parser_failed state,
               parser_complete comp):
    msg_(msg),
    failed_(state),
    complete_(comp) {}

  /** Copy constructor of #basic_parser.
   *
   * @param[in] other An existing instance of #basic_parser.
   */
  basic_parser(const basic_parser& other):
    msg_(other.msg_),
    failed_(other.failed_),
    complete_(other.complete_) {}

  /** Move constructor of #basic_parser.
   *
   * @param[in] other An existing instance of #basic_parser.
   */
  basic_parser(basic_parser&& other):
    msg_(std::move(other.msg_)),
    failed_(other.failed_),
    complete_(other.complete_) {}

  /** Whether this parser failed to fulfill expectations.
   */
  bool failed() const
  {
    return failed_ == parser_failed::failed;
  }

  /** Whether the parsing is complete.
   */
  bool complete() const
  {
    return complete_ == parser_complete::complete;
  }

  /** Whether this parser has failed or is complete.
   */
  bool failed_or_complete() const
  {
    return failed() || complete();
  }

  /** Whether this parser has not failed or is complete.
   */
  bool not_failed_or_complete() const
  {
    return (!failed()) || complete();
  }

  /** Whether this parser failed but is not complete.
   */
  bool failed_not_complete() const
  {
    return failed() && !complete();
  }

  /** The message beign parsed.
   */
  const message_type& message() const
  {
    return msg_;
  }
};

/** Returns a parser for the specified message.
 *
 * @param[in] msg The message beign parsed.
 * @return A parser.
 */
template <class TMsg>
inline basic_parser<TMsg> parse(const TMsg& msg)
{
  return basic_parser<TMsg>(msg);
}

/**  Returns a non-complete parser that failed to fulfill expectations.
 *
 * @param[in] msg The message beign parsed.
 *
 * @return A non-complete failed parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
fail(const TMsg& msg)
{
  typedef basic_parser<TMsg> parser_type;
  return parser_type(msg, parser_failed::failed);
}

/**  Returns a non-complete parser that failed to fulfill expectations.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] code The error code.
 * @param[in] err The error description.
 *
 * @return A non-complete failed parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
fail(const TMsg& msg, int code, const string& err)
{
  typedef basic_parser<TMsg> parser_type;
  return parser_type(msg.set_status(code, err), parser_failed::failed);
}

/**  Returns a non-complete parser that failed to fulfill expectations.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] code The error code.
 * @param[in] err The error description.
 *
 * @return A non-complete failed parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
fail(const TMsg& msg, http_code code, const string& err)
{
  typedef basic_parser<TMsg> parser_type;
  return parser_type(msg.set_status(code, err), parser_failed::failed);
}

/**  Returns a non-complete parser that failed to fulfill expectations.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] status A function that returns the error code and description.
 *
 * @return A non-complete failed parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
fail(const TMsg& msg, const typename TMsg::status_func& status)
{
  typedef basic_parser<TMsg> parser_type;
  return parser_type(msg.set_status(status), parser_failed::failed);
}

/**  Returns a non-failed complete parser and ends any further parsing.
 *
 * @param[in] msg The message beign parsed.
 *
 * @return A non-failed and complete parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
complete(const TMsg& msg)
{
  typedef basic_parser<TMsg> parser_type;
  return parser_type(msg, parser_failed::unknown, parser_complete::complete);
}

/**  Returns a non-failed complete parser and ends any further parsing.
 * Appends the specified response body content and sets the response status.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] code The status code.
 * @param[in] status_msg A descrptive status message.
 * @param[in] content_type The content type, may be empty.
 * @param[in] content The content to append.
 *
 * @return A non-failed and complete parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
complete_with_status(const TMsg& msg,
                     const int code,
                     const string& status_msg,
                     const string& content_type,
                     const string& content)
{
  typedef basic_parser<TMsg> parser_type;
  const TMsg msg_ = msg.output_status(code, status_msg, content_type, content);
  return parser_type(msg_, parser_failed::unknown, parser_complete::complete);
}

/**  Returns a non-failed complete parser and ends any further parsing.
 * Appends the specified response body content and sets the response status.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] status A function that returns the status.
 *
 * @return A non-failed and complete parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
complete_with_status(const TMsg& msg,
                     const typename TMsg::status_func& status,
                     const string& content_type,
                     const string& content)
{
  typedef basic_parser<TMsg> parser_type;
  const TMsg msg_ = msg.output_status(status, content_type, content);
  return parser_type(msg_, parser_failed::unknown, parser_complete::complete);
}

/**  Returns a non-failed complete parser and ends any further parsing.
 * Appends or update the WWW-Authenticate response header, sets
 * the 401 Unauthorized status and appends the specified response body content.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] challenge The Authrization challenge.
 * @param[in] content_type The content type, may be empty.
 * @param[in] content The content to append.
 *
 * @return A non-failed and complete parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
complete_with_challenge(const TMsg& msg,
                        const string& challenge,
                        const string& content_type,
                        const string& content)
{
  typedef basic_parser<TMsg> parser_type;
  const TMsg msg_ =
    msg.set_authentication_challenge(challenge, content_type, content);
  return parser_type(msg_, parser_failed::unknown, parser_complete::complete);
}

/**  Returns a non-failed complete parser and ends any further parsing.
 * Appends the specified response body content and sets the response status OK.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] content_type The content type, may be empty.
 * @param[in] content The content to append.
 *
 * @return A non-failed and complete parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
complete_ok(const TMsg& msg,
            const string& content_type,
            const string& content)
{
  return complete_with_status(msg, 200, "OK", content_type, content);
}

/**  Returns a non-failed complete parser and ends any further parsing.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] status The status.
 *
 * @return A non-failed and complete parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
complete_with_error(const TMsg& msg, const typename TMsg::status_type& status)
{
  typedef basic_parser<TMsg> parser_type;
  const TMsg msg_ = msg.set_status(status);
  return parser_type(msg_, parser_failed::unknown, parser_complete::complete);
}

/**  Returns a non-failed complete parser and ends any further parsing.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] code The error code.
 * @param[in] err The error description.
 *
 * @return A non-failed and complete parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
complete_with_error(const TMsg& msg, int code, const string& err)
{
  typedef basic_parser<TMsg> parser_type;
  const TMsg msg_ = msg.set_status(code, err);
  return parser_type(msg_, parser_failed::unknown, parser_complete::complete);
}

/**  Returns a non-failed complete parser and ends any further parsing.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] code The error code.
 * @param[in] err The error description.
 *
 * @return A non-failed and complete parser.
 */
template <class TMsg>
inline
basic_parser<TMsg>
complete_with_error(const TMsg& msg, http_code code, const string& err)
{
  typedef basic_parser<TMsg> parser_type;
  const TMsg msg_ = msg.set_status(code, err);
  return parser_type(msg_, parser_failed::unknown, parser_complete::complete);
}

/**  Returns a non-failed complete parser and ends any further parsing.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] status A function that returns the error code and description.
 *
 * @return A non-failed and complete parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
complete_with_error(const TMsg& msg, const typename TMsg::status_func& status)
{
  typedef basic_parser<TMsg> parser_type;
  const TMsg msg_ = msg.set_status(status);
  return parser_type(msg_, parser_failed::unknown, parser_complete::complete);
}

/** Appends the specified response body content and sets the response status OK.
 *  Returns a non-failed and non-complete parser.
 *
 * @param[in] msg The message beign parsed.
 * @param[in] content_type The content type, may be empty.
 * @param[in] content The content to append.
 *
 * @return A non-failed and non-complete parser.
 */
template <class TMsg>
inline basic_parser<TMsg>
continue_ok(const TMsg& msg,
            const string& content_type,
            const string& content)
{
  return parse(msg.output_status(200, "OK", content_type, content));
}

}
#endif
