/**
 * @file   content.hpp
 * @author jedf
 *
 * @brief  A filter that inspects the Content-Type header field.
 */
#pragma once
#ifndef HTPARSE_FILTERS_CONTENT_HPP_
#define HTPARSE_FILTERS_CONTENT_HPP_
#include <htparse/filter.hpp>
#include <string>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

namespace htparse { namespace filters {

using std::string;
using boost::function;
using boost::optional;
using boost::lexical_cast;
using boost::bad_lexical_cast;

/** A filter that checks the Content-Type header field.
 */
template <class TMsg>
class content:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

  /// The value of a header field.
  typedef optional<string> field;
  /// The value of the Content-Type field.
  typedef optional<string> ctype;
  /// The value of the Content-Length field.
  typedef optional<int> clength;
  /** A function that takes a message, a content type and a length
   * and returns a parser.
   */
  typedef function<parser_type (const message_type&,
                                const ctype&,
                                const clength&)> cfunc;

private:
  string type_field_;
  string length_field_;
  string expected_type_;
  cfunc func_;

public:
  /** Constructs an instance of #content.
   *
   * @param[in] func A function that evaluates the content type and length.
   */
  content(const cfunc& func):
    type_field_("Content-Type"),
    length_field_("Content-Length"),
    func_(func) {}

  /** Constructs an instance of #content.
   *
   * @param[in] expected_type The expected content type.
   */
  content(const string& expected_type):
    type_field_("Content-Type"),
    length_field_("Content-Length"),
    expected_type_(expected_type) {}

  /** Constructs an instance of #content.
   *
   * @param[in] expected_type The expected content type.
   * @param[in] func A function that evaluates the content type and length.
   */
  content(const string& expected_type,
          const cfunc& func):
    type_field_("Content-Type"),
    length_field_("Content-Length"),
    expected_type_(expected_type),
    func_(func) {}

  /** Copy constructor of #content.
   *
   * @param[in] other An existing instance of #content.
   */
  content(const content& other):
    type_field_(other.type_field_),
    length_field_(other.length_field_),
    expected_type_(other.expected_type_),
    func_(other.func_) {}

  /** Move constructor of #content.
   *
   * @param[in] other An existing instance of #content.
   */
  content(content&& other):
    type_field_(std::move(other.type_field_)),
    length_field_(std::move(other.length_field_)),
    expected_type_(std::move(other.expected_type_)),
    func_(std::move(other.func_)) {}

  /** Checks the expected content type, if any specified, and then invokes
   * the content function.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator()(const message_type& msg) const
  {
    const auto& req = msg.request();
    field type = req.get_header(type_field_);

    if(!expected_type_.empty())
    {
      if(expected_type_ != type)
        return fail(msg, set_error);
    }

    if(func_)
    {
      field length = req.get_header(length_field_);
      return func_(msg, type, parse_length(length));
    }

    return parse(msg);
  }

private:
  static
  status_type set_error()
  {
    return status_type(415, "Unsupported Media Type");
  }

  static
  clength parse_length(const field& v)
  {
    if(!v)
      return clength();
    const string& s = v.get();
    if(s.empty())
      return clength();
    try
    {
      return clength(lexical_cast<int>(s));
    }
    catch(const bad_lexical_cast&)
    {
      return clength();
    }
  }

};

}}
#endif
