#pragma once
#ifndef HTPARSE_BACKENDS_TESTING_RESPONSE_HPP_
#define HTPARSE_BACKENDS_TESTING_RESPONSE_HPP_
#include <htparse/backends/testing/result.hpp>
#include <ext/boost/optional.hpp>
#include <string>
#include <map>

namespace htparse { namespace testing {

using std::string;
using std::map;
using boost::optional;

template <class TBody>
class basic_response
{
public:
  typedef htparse::testing::result status_type;
  typedef TBody body_type;
  typedef string header_name_type;
  typedef string header_value_type;
  typedef map<header_name_type,
              header_value_type> header_type;

private:
  status_type status_;

  header_type header_;

  body_type body_;

public:
  basic_response() {}

  basic_response(const basic_response& other):
    status_(other.status_),
    header_(other.header_),
    body_(other.body_) {}

  basic_response(basic_response&& other):
    status_(std::move(other.status_)),
    header_(std::move(other.header_)),
    body_(std::move(other.body_)) {}

  void make_error_response(int code, const string& msg)
  {
    status_ = status_type(code, msg);
  }

  void status(int code, const string& msg)
  {
    status_ = status_type(code, msg);
  }

  const status_type& status() const
  {
    return status_;
  }

  void set_header(const string& name, const string& value)
  {
    if(!name.empty())
    {
      auto pos = header_.insert(std::make_pair(name, value));
      if (!pos.second)
        (pos.first)->second = value;
    }
  }

  template<class TDict>
  void set_headers(const TDict& dict)
  {
    for(const auto& p : dict)
    {
      set_header(p.first, p.second);
    }
  }

  optional<string> get_header(const string& p) const
  {
    auto pos = header_.find(p);
    if (pos == header_.end())
      return optional<string>();
    return optional<string>(pos->second);
  }

  void content_type(const string& ctype)
  {
    set_header("Content-Type", ctype);
  }

  string body() const
  {
    return body_.str();
  }

  body_type& out()
  {
    return body_;
  }

  void out(const optional<string>& ctype, const string& body)
  {
    if(ctype.is_initialized())
      out(ctype.get(), body);
    else
      body_ << body;
  }

  void out(const string& ctype, const string& body)
  {
    if(!ctype.empty())
      content_type(ctype);
    body_ << body;
  }

};

}}
#endif
