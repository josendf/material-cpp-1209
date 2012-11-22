/**
 * @file   auth.hpp
 * @author jedf
 *
 * @brief  A filter that checks the Authorization header.
 */
#pragma once
#ifndef HTPARSE_FILTERS_AUTH_HPP_
#define HTPARSE_FILTERS_AUTH_HPP_
#include <htparse/filter.hpp>
#include <htparse/auth/mac.hpp>
#include <string>
#include <boost/function.hpp>
#include <boost/optional.hpp>

namespace htparse { namespace filters {

using std::string;
using boost::function;
using boost::optional;

/** A filter that checks the Authorization header field and if it is
 *  present invokes a  function that returns a parser.
 *  Otherwise returns the specified Authentication challenge.
 */
template <class TMsg>
class authorization
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
                                const string&)> hfunc;

private:
  string challenge_;
  hfunc func_;

public:

  /** Constructs an instance of #authorization.
   */
  authorization() {}

  /** Constructs an instance of #authorization.
   *
   * @param[in] challenge The authentication challenge.
   */
  authorization(const string& challenge):
    challenge_(challenge) {}

  /** Constructs an instance of #authorization.
   *
   * @param[in] challenge The authentication challenge.
   * @param[in] func A function that evaluates the Authorization
   * header field value.
   */
  authorization(const string& challenge,
                const hfunc& func):
    challenge_(challenge),
    func_(func) {}

  /** Copy constructor of #authorization.
   *
   * @param[in] other An existing instance of #authorization.
   */
  authorization(const authorization& other):
    challenge_(other.challenge_),
    func_(other.func_) {}

  /** Move constructor of #authorization.
   *
   * @param[in] other An existing instance of #authorization.
   */
  authorization(authorization&& other):
    challenge_(std::move(move(other.challenge_))),
    func_(std::move(other.func_)) {}

  /** Checks the Authorization header field and then invokes
   * the header field function.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator()(const message_type& msg) const
  {
    field v = msg.request().get_header("Authorization");
    if(v)
    {
      if(func_)
        return func_(msg, v.get());
      return parse(msg);
    }

    if(!challenge_.empty())
      return fail(msg.set_authentication_challenge(challenge_));

    return fail(msg, set_error);
  }

private:
  static
  status_type set_error()
  {
    return status_type(401, "Unauthorized");
  }
};

}}
#endif
