/**
 * @file   mac_auth.hpp
 * @author jedf
 *
 * @brief  A filter that checks the MAC Authorization header.
 */
#pragma once
#ifndef HTPARSE_FILTERS_MAC_AUTH_HPP_
#define HTPARSE_FILTERS_MAC_AUTH_HPP_
#include <htparse/filter.hpp>
#include <htparse/auth/mac.hpp>
#include <htparse/helpers.hpp>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <string>

namespace htparse { namespace filters {
using std::string;
using std::pair;
using boost::function;
using boost::optional;

/** A filter that checks the MAC Authorization header field and if
 *  it is present invokes a  function that returns a parser.
 * Otherwise returns the MAC Authentication challenge.
 */
template <class TMsg>
class mac_auth
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

  /// The value of the MAC header field.
  typedef htparse::auth::mac::header header;

  /// The normalized request string.
  typedef htparse::auth::mac::norm_request norm_request;

  /// The value of a header field.
  typedef optional<string> field;

  /// A function that takes a header field and returns a parser.
  typedef function<parser_type (const message_type&,
                                const header&,
                                const norm_request&)> hfunc;

private:
  htparse::auth::mac::header_parser hdr_parser_;
  hfunc func_;

public:
  /** Constructs an instance of #mac_auth.
   *
   * @param[in] challenge The authentication challenge.
   * @param[in] func A function that evaluates the Authorization
   * header field value.
   */
  mac_auth(const hfunc& func):
    func_(func) {}

  /** Copy constructor of #mac_auth.
   *
   * @param[in] other An existing instance of #mac_auth.
   */
  mac_auth(const mac_auth& other):
    func_(other.func_) {}

  /** Move constructor of #mac_auth.
   *
   * @param[in] other An existing instance of #mac_auth.
   */
  mac_auth(mac_auth&& other):
    func_(std::move(other.func_)) {}

  /** Checks the Authorization header field and then invokes
   * the header field function.
   *
   * @param[in] msg The message being parsed.
   * @return Either a succeeded or failed parser.
   */
  parser_type operator()(const message_type& msg) const
  {
    using htparse::helpers::host_name_and_port;

    const auto& req = msg.request();
    field v = req.get_header("Authorization");
    if(!v || v.get().empty())
      return complete_with_challenge(msg,
                                     "MAC",
                                     "text/plain",
                                     "Please provide the "
                                     "MAC Authorization header\r\n");

    optional<header> h = hdr_parser_.parse(v.get());
    if(!h)
      return complete_with_status(msg, 400, "Bad Request",
                                  "text/plain",
                                  "The MAC Authorization header "
                                  "is malformed or not valid\r\n");

    if(!func_)
      return parse(msg);

    // The normalized request string is a consistent, reproducible
    // concatenation of several of the HTTP request elements into a single
    // string. By normalizing the request into a reproducible string, the
    // client and server can both calculate the request MAC over the exact
    // same value.
    string req_method;
    string req_uri;
    pair<string, int> hst;

    // Para permitir exponer los servicios que requieren autorización
    // mediante proxys introducimos los siguientes encabezados:
    //
    // X-Auth-Method Método HTTP de la petición original
    // X-Auth-Host   Contenido del encabezado Host original
    // X-Auth-URL    URL de la petición original
    //
    field xm = req.get_header("X-Auth-Method");
    if(!xm || xm.get().empty())
    {
      req_method = req.method();
      req_uri = req.uri();
      hst = req.host_name_and_port();
    }
    else
    {
      req_method = xm.get();
      field xh = req.get_header("X-Auth-Host");
      if(xh)
        hst = host_name_and_port(xh.get(), req.port());
      field xu = req.get_header("X-Auth-URL");
      if(xu)
        req_uri = xu.get();
    }

    norm_request nrs(h.get(), req_method, hst.first, hst.second, req_uri);

    return func_(msg, h.get(), nrs);

  }
};

}}
#endif
