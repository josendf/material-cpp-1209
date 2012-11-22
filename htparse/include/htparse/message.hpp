/**
 * @file   message.hpp
 * @author jedf
 *
 * @brief  An HTTP message that is beign parsed.
 */
#pragma once
#ifndef HTPARSE_MESSAGE_HPP_
#define HTPARSE_MESSAGE_HPP_
#include <htparse/status.hpp>
#include <string>
#include <ext/boost/shared_ptr.hpp>
#include <ext/boost/function.hpp>
#include <ext/boost/optional.hpp>
#include <map>

namespace htparse {

using std::string;
using std::map;
using boost::shared_ptr;
using boost::function;
using boost::optional;

/** An HTTP message that is beign parsed.
 */
template <class TCtx>
class basic_message
{
public:
  /// The HTTP context object type.
  typedef TCtx context_type;
  /// The HTTP context pointer type.
  typedef shared_ptr<context_type> context_ptr;
  /// The request object type.
  typedef typename TCtx::request_type request_type;
  /// The response object type.
  typedef typename TCtx::response_type response_type;
  /// The response body type.
  typedef string response_body_type;
  /// The status type.
  typedef htparse::status status_type;
  /// The response header name type.
  typedef string header_name_type;
  /// The response header value type.
  typedef string header_value_type;
  /// The response header dictionary type.
  typedef map<header_name_type,
              header_value_type> response_header_type;
  /// A function that returns a status.
  typedef function<status_type ()> status_func;

private:
  context_ptr context_;

  status_func status_;

  response_header_type response_header_;

  response_body_type response_body_;

public:
  /** Constructs an of #basic_message.
   *
   * @param[in] context The HTTP transaction context.
   */
  basic_message(context_ptr context):
    context_(context) {}

  /** Copy constructor of #basic_message.
   *
   * @param[in] other An existing instance of #basic_message.
   */
  basic_message(const basic_message& other):
    context_(other.context_),
    status_(other.status_),
    response_header_(other.response_header_),
    response_body_(other.response_body_) {}

  /** Move constructor of #basic_message.
   *
   * @param[in] other An existing instance of #basic_message.
   */
  basic_message(basic_message&& other):
    context_(std::move(other.context_)),
    status_(std::move(other.status_)),
    response_header_(std::move(other.response_header_)),
    response_body_(std::move(other.response_body_)) {}

  /** The HTTP request object.
   */
  const request_type& request() const
  {
    return context_->request();
  }

  /** The status of this message.
   */
  status_type status() const
  {
    if(!status_)
      return status_type();
    return status_();
  }

  /** Modifies the status of this message.
   *
   * @param[in] func A function that returns the new status.
   * @return A new instance of #basic_message with the status modified.
   */
  basic_message set_status(const status_func& func) const
  {
    basic_message other(*this);
    other.reset_status(func);
    return other;
  }

  /** Modifies the status of this message.
   *
   * @param[in] status The new status.
   * @return A new instance of #basic_message with the status modified.
   */
  basic_message set_status(const status_type& status) const
  {
    basic_message other(*this);
    other.reset_status(status);
    return other;
  }

  /** Modifies the status of this message.
   *
   * @param[in] code The status code.
   * @param[in] msg A descriptive message.
   * @return A new instance of #basic_message with the status modified.
   */
  basic_message set_status(const int code, const string& msg) const
  {
    basic_message other(*this);
    other.reset_status(code, msg);
    return other;
  }

  /** Modifies the status of this message.
   *
   * @param[in] code The status code.
   * @param[in] msg A descriptive message.
   * @return A new instance of #basic_message with the status modified.
   */
  basic_message set_status(const http_code code, const string& msg) const
  {
    basic_message other(*this);
    other.reset_status(code, msg);
    return other;
  }

  /** Appends or update the response headers of the message.
   */
  template <class TDict>
  basic_message set_response_headers(const TDict& dict) const
  {
    using std::make_pair;
    basic_message other(*this);
    for(const auto& p : dict)
    {
      if(!p.first.empty())
      {
        auto pos = other.response_header_.insert(make_pair(p.first, p.second));
        if (!pos.second)
          (pos.first)->second = p.second;
      }
    }
    return other;
  }

  /** Appends or update the response headers of the message.
   */
  template <class TDict>
  basic_message set_response_header(const string& name,
                                    const string& value) const
  {
    using std::make_pair;
    basic_message other(*this);
    if(!name.empty())
    {
      auto pos = other.response_header_.insert(make_pair(name, value));
      if (!pos.second)
        (pos.first)->second = value;
    }
    return other;
  }

  /** Appends or update the WWW-Authenticate response header
   * and sets the 401 Unauthorized status.
   */
  basic_message set_authentication_challenge(const string& challenge) const
  {
    using std::make_pair;
    basic_message other(*this);
    other
      .set_response_header("WWW-Authenticate", challenge)
      .reset_status(401, "Unauthorized");
    return other;
  }

  /** Appends or update the WWW-Authenticate response header
   * and sets the 401 Unauthorized status.
   */
  basic_message set_authentication_challenge(const string& challenge,
                                             const string& content_type,
                                             const string& content) const
  {
    using std::make_pair;
    basic_message other(*this);
    other
      .set_response_header("WWW-Authenticate", challenge)
      .reset_status(401, "Unauthorized")
      .append_content(content_type, content);
    return other;
  }

  /** Gets the response header dictionary
   */
  const response_header_type& response_header() const
  {
    return response_header_;
  }

  /** Get a response header value.
   */
  optional<string> response_header(const string& name) const
  {
    auto pos = response_header_.find(name);
    if (pos == response_header_.end())
      return optional<string>();
    return optional<string>(pos->second);
  }

  /** Checks the specified response header.
   */
  bool has_response_header(const string& name) const
  {
    const auto pos = response_header_.find(name);
    return pos != response_header_.end();
  }

  /** Gets the response body content type.
   */
  optional<string> response_content_type() const
  {
    return response_header("Content-Type");
  }

  /** Checks the response body content type.
   */
  bool has_response_content_type(const string& ctype) const
  {
    const optional<string> ct = response_content_type();
    if(!ct)
      return false;
    return ct.get() == ctype;
  }

  /** Checks the response body content type.
   */
  bool has_response_content_type() const
  {
    const optional<string> ct = response_content_type();
    if(!ct)
      return false;
    return !ct.get().empty();
  }

  /** The response body content.
   */
  const response_body_type& response_body() const
  {
    return response_body_;
  }

  /** Append some content to the response body.
   *
   * @param[in] content The content to append.
   * @return A new instance of #basic_message with the new content appended.
   */
  basic_message output(const string& content_type,
                       const string& content) const
  {
    basic_message other(*this);
    other.append_content(content_type, content);
    return other;
  }

  /** Modifies the status of this message and
   * append some content to the response body.
   *
   * @param[in] func A function that returns the new status.
   * @param[in] content_type The content type, may be empty.
   * @param[in] content The content to append.
   * @return A new instance of #basic_message with the status modified
   * and the new content appended.
   */
  basic_message output_status(const status_func& func,
                              const string& content_type,
                              const string& content) const
  {
    basic_message other(*this);
    other
      .append_content(content_type, content)
      .reset_status(func);
    return other;
  }

  /** Modifies the status of this message and
   * append some content to the response body.
   *
   * @param[in] code The status code.
   * @param[in] msg A descrptive message.
   * @param[in] content_type The content type, may be empty.
   * @param[in] content The content to append.
   * @return A new instance of #basic_message with the status modified
   * and the new content appended.
   */
  basic_message output_status(const int code,
                              const string& msg,
                              const string& content_type,
                              const string& content) const
  {
    basic_message other(*this);
    other
      .append_content(content_type, content)
      .reset_status(code, msg);
    return other;
  }

  /** Modifies the status of this message and
   * append some content to the response body.
   *
   * @param[in] code The status code.
   * @param[in] msg A descrptive message.
   * @param[in] content_type The content type, may be empty.
   * @param[in] content The content to append.
   * @return A new instance of #basic_message with the status modified
   * and the new content appended.
   */
  basic_message output_status(const http_code code,
                              const string& msg,
                              const string& content_type,
                              const string& content) const
  {
    basic_message other(*this);
    other
      .append_content(content_type, content)
      .reset_status(code, msg);
    return other;
  }


private:
  //
  // Keep the public interface read-only
  //

  basic_message&
  reset_status(const status_func& func)
  {
    status_ = func;
    return *this;
  }

  basic_message&
  reset_status(const status_type& status)
  {
    status_ = [=](){ return status; };
    return *this;
  }

  basic_message&
  reset_status(const int code, const string& msg)
  {
    status_ = [=](){ return status_type(code, msg); };
    return *this;
  }

  basic_message&
  reset_status(const http_code code, const string& msg)
  {
    status_ = [=](){ return status_type(code, msg); };
    return *this;
  }

  basic_message&
  append_content(const string& content_type, const string& content)
  {
    set_response_content_type(content_type);
    response_body_.append(content);
    return *this;
  }

  /** Sets the response body content type.
   */
  basic_message&
  set_response_content_type(const string& ctype)
  {
    if(!ctype.empty())
      set_response_header("Content-Type", ctype);
    return *this;
  }

  /** Sets a response header value.
   */
  basic_message&
  set_response_header(const string& name, const string& value)
  {
    if(!name.empty())
    {
      auto pos = response_header_.insert(std::make_pair(name, value));
      if (!pos.second)
        (pos.first)->second = value;
    }
    return *this;
  }


};

}
#endif
