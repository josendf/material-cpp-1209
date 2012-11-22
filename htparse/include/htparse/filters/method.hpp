/**
 * @file   method.hpp
 * @author jedf
 *
 * @brief  A filter that checks the HTTP Method.
 */
#pragma once
#ifndef HTPARSE_FILTERS_METHOD_HPP_
#define HTPARSE_FILTERS_METHOD_HPP_
#include <htparse/filter.hpp>
#include <string>
#include <vector>
#include <algorithm>

namespace htparse { namespace filters {

using std::string;
using std::vector;

/** A filter that checks the HTTP Method.
 */
template <class TMsg>
class method:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

private:
  vector<string> methods_;

public:
  /** Constructs an instance of #method.
   *
   * @param[in] method The HTTP method to check.
   */
  method(const string& method):
    methods_(1, method) {}

  /** Constructs an instance of #method.
   *
   * @param[in] m_1 The first HTTP method to check.
   * @param[in] m_2 The second HTTP method to check.
   */
  method(const string& m_1,
               const string& m_2)
  {
    methods_.reserve(2);
    methods_.push_back(m_1);
    methods_.push_back(m_2);
  }

  /** Constructs an instance of #method.
   *
   * @param[in] m_1 The first HTTP method to check.
   * @param[in] m_2 The second HTTP method to check.
   * @param[in] m_3 The third HTTP method to check.
   */
  method(const string& m_1,
               const string& m_2,
               const string& m_3)
  {
    methods_.reserve(3);
    methods_.push_back(m_1);
    methods_.push_back(m_2);
    methods_.push_back(m_3);
  }

  /** Constructs an instance of #method.
   *
   * @param[in] m_1 The first HTTP method to check.
   * @param[in] m_2 The second HTTP method to check.
   * @param[in] m_3 The third HTTP method to check.
   * @param[in] m_4 The fourth HTTP method to check.
   */
  method(const string& m_1,
               const string& m_2,
               const string& m_3,
               const string& m_4)
  {
    methods_.reserve(4);
    methods_.push_back(m_1);
    methods_.push_back(m_2);
    methods_.push_back(m_3);
    methods_.push_back(m_4);
  }

  method(std::initializer_list<std::string> ms):
    methods_(ms) {}

  /** Copy constructor of #method.
   *
   * @param[in] other An existing instance of #basic_content.
   */
  method(const method& other):
    methods_(other.methods_) {}

  /** Move constructor of #method.
   *
   * @param[in] other An existing instance of #basic_content.
   */
  method(method&& other):
    methods_(std::move(other.methods_)) {}

  /** Checks the expected HTTP method and returns a parser.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator()(const message_type& msg) const
  {
    const string method(msg.request().method());
    auto pos = std::find(methods_.begin(), methods_.end(), method);
    if(pos == methods_.end())
      return fail(msg, set_error);
    return parse(msg);
  }

private:
  static
  status_type set_error()
  {
    return status_type(405, "Method Not Allowed");
  }

};

}}
#endif
