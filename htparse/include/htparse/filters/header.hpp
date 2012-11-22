/**
 * @file   header.hpp
 * @author jedf
 *
 * @brief  A filter that checks the specified header fields.
 */
#pragma once
#ifndef HTPARSE_FILTERS_HEADER_HPP_
#define HTPARSE_FILTERS_HEADER_HPP_
#include <htparse/filter.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/function.hpp>
#include <boost/optional.hpp>

namespace htparse { namespace filters {

using std::string;
using std::vector;
using std::size_t;
using boost::function;
using boost::optional;

/** A filter that extracts the specified header fields and then invokes a
 *  function that returns a parser.
 */
template <class TMsg>
class header
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

  /// The value of a header field.
  typedef optional<string> field;

  /// A function that takes a header field and returns a parser.
  typedef function<parser_type (const message_type&,
                                const field&)> hfunc_1;
  /// A function that takes two header fields and returns a parser.
  typedef function<parser_type (const message_type&,
                                const field&,
                                const field&)> hfunc_2;
  /// A function that takes three header fields and returns a parser.
  typedef function<parser_type (const message_type&,
                                const field&, const field&,
                                const field&)> hfunc_3;

private:
  hfunc_1 func_1_;
  hfunc_2 func_2_;
  hfunc_3 func_3_;

  vector<string> fields_;

public:

  /** Constructs an instance of #header.
   *
   * @param[in] field The name of a header field.
   */
  header(const string& field)
  {
    fields_.push_back(field);
  }

  /** Constructs an instance of #header.
   *
   * @param[in] field The name of a header field.
   * @param[in] func A function that evaluates the header field value.
   */
  header(const string& field,
         const hfunc_1& func):
    func_1_(func)
  {
    fields_.push_back(field);
  }

  /** Constructs an instance of #header.
   *
   * @param[in] f_1 The name of the first header field.
   * @param[in] f_2 The name of the second header field.
   */
  header(const string& f_1,
         const string& f_2)
  {
    fields_.reserve(2);
    fields_.push_back(f_1);
    fields_.push_back(f_2);
  }

  /** Constructs an instance of #header.
   *
   * @param[in] f_1 The name of the first header field.
   * @param[in] f_2 The name of the second header field.
   * @param[in] func A function that evaluates the header field values.
   */
  header(const string& f_1,
         const string& f_2,
         const hfunc_2& func):
    func_2_(func)
  {
    fields_.reserve(2);
    fields_.push_back(f_1);
    fields_.push_back(f_2);
  }

  /** Constructs an instance of #header.
   *
   * @param[in] f_1 The name of the first header field.
   * @param[in] f_2 The name of the second header field.
   * @param[in] f_3 The name of the third header field.
   */
  header(const string& f_1,
         const string& f_2,
         const string& f_3)
  {
    fields_.reserve(3);
    fields_.push_back(f_1);
    fields_.push_back(f_2);
    fields_.push_back(f_3);
  }

  /** Constructs an instance of #header.
   *
   * @param[in] f_1 The name of the first header field.
   * @param[in] f_2 The name of the second header field.
   * @param[in] f_3 The name of the third header field.
   * @param[in] func A function that evaluates the header field values.
   */
  header(const string& f_1,
         const string& f_2,
         const string& f_3,
         const hfunc_3& func):
    func_3_(func)
  {
    fields_.reserve(3);
    fields_.push_back(f_1);
    fields_.push_back(f_2);
    fields_.push_back(f_3);
  }

  /** Copy constructor of #header.
   *
   * @param[in] other An existing instance of #header.
   */
  header(const header& other):
    func_1_(other.func_1_),
    func_2_(other.func_2_),
    func_3_(other.func_3_),
    fields_(other.fields_) {}

  /** Move constructor of #header.
   *
   * @param[in] other An existing instance of #header.
   */
  header(header&& other):
    func_1_(std::move(other.func_1_)),
    func_2_(std::move(other.func_2_)),
    func_3_(std::move(other.func_3_)),
    fields_(std::move(other.fields_)) {}

  /** Checks the expected header fields and then invokes
   * the header field function.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator()(const message_type& msg) const
  {
    const std::size_t n = fields_.size();
    if (n < 1)
      return parse(msg);

    field v1 = msg.request().get_header(fields_[0]);
    if(n < 2)
    {
      if(func_1_)
        return func_1_(msg, v1);
      else if(!v1)
        fail(msg, set_error);
    }
    else
    {
      field v2 = msg.request().get_header(fields_[1]);
      if(n < 3)
      {
        if(func_2_)
          return func_2_(msg, v1, v2);
        else if(!v2)
          fail(msg, set_error);
      }
      else
      {
        field v3 = msg.request().get_header(fields_[2]);
        if(func_3_)
          return func_3_(msg, v1, v2, v3);
        else if(!v3)
          fail(msg, set_error);
      }
    }

    return parse(msg);
  }

private:
  static
  status_type set_error()
  {
    return status_type(400, "Bad Request");
  }
};

}}
#endif
