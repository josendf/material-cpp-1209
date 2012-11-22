/**
 * @file   bearer_auth.hpp
 * @author jedf
 *
 * @brief  A filter that checks the Bearer Authorization header.
 */
#pragma once
#ifndef HTPARSE_FILTERS_BEARER_AUTH_HPP_
#define HTPARSE_FILTERS_BEARER_AUTH_HPP_
#include <htparse/filter.hpp>
#include <htparse/auth/bearer.hpp>
#include <string>
#include <boost/function.hpp>
#include <boost/optional.hpp>

namespace htparse { namespace filters {
using std::string;
using boost::function;
using boost::optional;

/** A filter that checks the Bearer Authorization header field and if
 *  it is present invokes a  function that returns a parser.
 * Otherwise returns the Bearer Authentication challenge.
 */
template <class TMsg>
class bearer_auth
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

  /// The value of the Bearer Authorization token
  typedef string token;

  /// The value of a header field.
  typedef optional<string> field;

  /// A function that takes a header field and returns a parser.
  typedef function<parser_type (const message_type&,
                                const token&)> hfunc;

private:
  htparse::auth::bearer::header_parser hdr_parser_;

  hfunc func_;

public:
  /** Constructs an instance of #bearer_auth.
   *
   * @param[in] challenge The authentication challenge.
   * @param[in] func A function that evaluates the Authorization
   * header field value.
   */
  bearer_auth(const hfunc& func):
    func_(func) {}

  /** Copy constructor of #bearer_auth.
   *
   * @param[in] other An existing instance of #bearer_auth.
   */
  bearer_auth(const bearer_auth& other):
    func_(other.func_) {}

  /** Move constructor of #bearer_auth.
   *
   * @param[in] other An existing instance of #bearer_auth.
   */
  bearer_auth(bearer_auth&& other):
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
    if(!v || v.get().empty())
      return complete_with_challenge(msg,
                                     "Bearer",
                                     "text/plain",
                                     "Please provide the "
                                     "Bearer Authorization header\r\n");

    optional<token> tk = hdr_parser_.parse(v.get());
    if(!tk)
      return complete_with_status(msg, 400, "Bad Request",
                                  "text/plain",
                                  "The Bearer Authorization header "
                                  "is malformed or not valid\r\n");

    if(!func_)
      return parse(msg);

    return func_(msg, tk.get());

  }
};

}}
#endif
