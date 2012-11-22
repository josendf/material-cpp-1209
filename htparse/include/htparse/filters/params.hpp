/**
 * @file   params.hpp
 * @author jedf
 *
 * @brief  A filter that extracts the specified request parameters and
 * executes a function.
 */
#pragma once
#ifndef HTPARSE_FILTERS_PARAMS_HPP_
#define HTPARSE_FILTERS_PARAMS_HPP_
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

/**  A filter that extracts the specified request parameters and
 * executes a function.
 */
template <class TMsg>
class params:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

  typedef optional<string> param;

  typedef function<parser_type (const message_type&,
                                const param&)> func_1;
  typedef function<parser_type (const message_type&,
                                const param&,
                                const param&)> func_2;
  typedef function<parser_type (const message_type&,
                                const param&,
                                const param&,
                                const param&)> func_3;

private:
  func_1 func_1_;
  func_2 func_2_;
  func_3 func_3_;

  vector<string> params_;

public:
  /** Constructs an instance of #params.
   *
   * @param[in] p_1 The name of the parameter.
   * @param[in] func The filter function.
   */
  params(const std::string& p_1,
         const func_1& func):
    func_1_(func)
  {
    params_.push_back(p_1);
  }

  /** Constructs an instance of #params.
   *
   * @param[in] p_1 The name of the first parameter.
   * @param[in] p_2 The name of the second parameter.
   * @param[in] func The filter function.
   */
  params(const std::string& p_1,
         const std::string& p_2,
         const func_2& func):
    func_2_(func)
  {
    params_.reserve(2);
    params_.push_back(p_1);
    params_.push_back(p_2);
  }

  /** Constructs an instance of #params.
   *
   * @param[in] p_1 The name of the first parameter.
   * @param[in] p_2 The name of the second parameter.
   * @param[in] p_2 The name of the third parameter.
   * @param[in] func The filter function.
   */
  params(const std::string& p_1,
         const std::string& p_2,
         const std::string& p_3,
         const func_3& func):
    func_3_(func)
  {
    params_.reserve(3);
    params_.push_back(p_1);
    params_.push_back(p_2);
    params_.push_back(p_3);
  }

  /** Copy constructor of #params.
   *
   * @param[in] other An existing instance of #params.
   */
  params(const params& other):
    func_1_(other.func_1_),
    func_2_(other.func_2_),
    func_3_(other.func_3_),
    params_(other.params_) {}

  /** Move constructor of #params.
   *
   * @param[in] other An existing instance of #params.
   */
  params(params&& other):
    func_1_(std::move(other.func_1_)),
    func_2_(std::move(other.func_2_)),
    func_3_(std::move(other.func_3_)),
    params_(std::move(other.params_)) {}

  /** Executes to specified filter passing the specified request
   * parameters values.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator()(const message_type& msg) const
  {
    const std::size_t n = params_.size();
    if (n < 1)
      return parse(msg);

    param v1 = msg.request().get_param(params_[0]);
    if(n < 2)
    {
      if(func_1_)
        return func_1_(msg, v1);
    }
    else
    {
      param v2 = msg.request().get_param(params_[1]);
      if(n < 3)
      {
        if(func_2_)
          return func_2_(msg, v1, v2);
      }
      else
      {
        param v3 = msg.request().get_param(params_[2]);
        if(func_3_)
          return func_3_(msg, v1, v2, v3);
      }
    }

    return parse(msg);
  }
};

}}
#endif
