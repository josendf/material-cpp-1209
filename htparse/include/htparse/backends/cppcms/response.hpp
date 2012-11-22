#pragma once
#ifndef HTPARSE_BACKENDS_CPPCMS_RESPONSE_HPP_
#define HTPARSE_BACKENDS_CPPCMS_RESPONSE_HPP_
#include <htparse/status.hpp>
#include <string>
#include <ostream>
#include <ext/boost/optional.hpp>

namespace htparse { namespace cppcms {

using std::string;
using std::ostream;
using boost::optional;

template <class TRawResp>
class basic_response
{
public:
  typedef TRawResp raw_response;

  typedef raw_response* raw_response_ptr;

private:
  raw_response_ptr raw_response_;

public:
  basic_response(raw_response_ptr resp):
    raw_response_(resp) {}

  void make_error_response(int code, const string& msg)
  {
    raw_response_->make_error_response(code, msg);
  }

  void status(int code, const string& msg)
  {
    raw_response_->status(code, msg);
  }

  void content_type(const string& ctype)
  {
    raw_response_->content_type(ctype);
  }

  ostream& out()
  {
    return raw_response_->out();
  }

  void out(const optional<string>& ctype, const string& body)
  {
    if(ctype.is_initialized())
      out(ctype.get(), body);
    else
      raw_response_->out() << body;
  }

  void out(const string& ctype, const string& body)
  {
    if(!ctype.empty())
      raw_response_->content_type(ctype);
    raw_response_->out() << body;
  }

  void set_header(const string& name, const string& value)
  {
    if(!name.empty())
      raw_response_->set_header(name, value);
  }

  template<class TDict>
  void set_headers(const TDict& dict)
  {
    for(const auto& p : dict)
    {
      set_header(p.first, p.second);
    }
  }


};

}}
#endif
