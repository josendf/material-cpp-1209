#pragma once
#ifndef HTPARSE_BACKENDS_TESTING_REQUEST_HPP_
#define HTPARSE_BACKENDS_TESTING_REQUEST_HPP_
#include <map>
#include <string>
#include <utility>
#include <ext/boost/optional.hpp>

namespace htparse { namespace testing {
using std::map;
using std::string;
using std::pair;
using std::make_pair;
using boost::optional;

template <class TBody>
class basic_request
{
public:
  typedef TBody body_type;

private:
  string method_;

  string host_;

  string host_name_;

  int port_;

  string qs_;

  string path_;

  map<string, string> header_;

  map<string, string> params_;

  body_type body_;

public:
  basic_request() {}

  basic_request(const basic_request& other):
    method_(other.method_),
    host_(other.host_),
    host_name_(other.host_name_),
    port_(other.port_),
    qs_(other.qs_),
    path_(other.path_),
    header_(other.header_),
    params_(other.params_),
    body_(other.body_) {}

  basic_request(basic_request&& other):
    method_(std::move(other.method_)),
    host_(std::move(other.host_)),
    host_name_(std::move(other.host_name_)),
    port_(other.port_),
    qs_(std::move(other.qs_)),
    path_(std::move(other.path_)),
    header_(std::move(other.header_)),
    params_(std::move(other.params_)),
    body_(std::move(other.body_)) {}

  basic_request& operator =(const basic_request& other)
  {
    if(this != &other)
    {
      method_ = other.method_;
      host_   = other.host_;
      host_name_   = other.host_name_;
      port_   = other.port_;
      qs_     = other.qs_;
      path_ = other.path_;
      header_ = other.header_;
      params_ = other.params_;
      body_ = other.body_;
    }
    return *this;
  }

  basic_request& operator =(basic_request&& other)
  {
    if(this != &other)
    {
      method_ = std::move(other.method_);
      host_   = std::move(other.host_);
      host_name_   = std::move(other.host_name_);
      port_   = other.port_;
      qs_     = std::move(other.qs_);
      path_   = std::move(other.path_);
      header_ = std::move(other.header_);
      params_ = std::move(other.params_);
      body_   = std::move(other.body_);
    }
    return *this;
  }

  const string& method() const
  {
    return method_;
  }

  string& method()
  {
    return method_;
  }

  const string& host() const
  {
    return host_;
  }

  string& host()
  {
    return host_;
  }

  const string& host_name() const
  {
    return host_name_;
  }

  string& host_name()
  {
    return host_name_;
  }

  int port() const
  {
    return port_;
  }

  int& port()
  {
    return port_;
  }

  pair<string, int> host_name_and_port() const
  {
    return make_pair(host_name_, port_);
  }

  const string& query_string() const
  {
    return qs_;
  }

  string& query_string()
  {
    return qs_;
  }

  const string& path() const
  {
    return path_;
  }

  string& path()
  {
    return path_;
  }

  string uri() const
  {
    string u(path_);
    if(!qs_.empty())
      u.append(1, '?').append(qs_);
    return u;
  }

  void set_param(const string& p, const string& v)
  {
    auto pos = params_.insert(std::make_pair(p, v));
    if (!pos.second)
      (pos.first)->second = v;
  }

  optional<string> get_param(const string& p) const
  {
    auto pos = params_.find(p);
    if (pos == params_.end())
      return optional<string>();
    return optional<string>(pos->second);
  }

  void set_header(const string& p, const string& v)
  {
    auto pos = header_.insert(std::make_pair(p, v));
    if (!pos.second)
      (pos.first)->second = v;
  }

  optional<string> get_header(const string& p) const
  {
    auto pos = header_.find(p);
    if (pos == header_.end())
      return optional<string>();
    return optional<string>(pos->second);
  }
};

}}
#endif
