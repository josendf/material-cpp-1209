#pragma once
#ifndef HTPARSE_BACKENDS_CPPCMS_REQUEST_HPP_
#define HTPARSE_BACKENDS_CPPCMS_REQUEST_HPP_
#include <string>
#include <cstring>
#include <cstdlib>
#include <utility>
#include <locale>
#include <ext/boost/optional.hpp>
#include <htparse/helpers.hpp>

namespace htparse { namespace cppcms {

using std::string;
using std::pair;
using std::make_pair;
using boost::optional;

template <class TRawReq>
class basic_request
{
public:
  typedef TRawReq raw_request_type;
  typedef TRawReq* raw_request_ptr;

private:
  raw_request_ptr raw_request_;

public:
  basic_request(raw_request_ptr req):
    raw_request_(req) {}

  string method() const
  {
    return raw_request_->request_method();
  }

  string host() const
  {
    return raw_request_->http_host();
  }

  pair<string, int> host_name_and_port() const
  {
    const string host_header(raw_request_->http_host());
    const int default_port =
      static_cast<int>(raw_request_->server_port());

    const pair<string, int> hnp =
      htparse::helpers::host_name_and_port(host_header, default_port);

    if(hnp.first.empty())
      return make_pair(raw_request_->server_name(), default_port);

    return hnp;
  }

  string host_name() const
  {
    const pair<string, int> hp = host_name_and_port();
    return hp.first;
  }

  int port() const
  {
    const pair<string, int> hp = host_name_and_port();
    return hp.second;
  }

  string path() const
  {
    return raw_request_->path_info();
  }

  string query_string() const
  {
    return raw_request_->query_string();
  }

  string uri() const
  {
    string ruri = raw_request_->getenv("REQUEST_URI");
    if(ruri.empty())
    {
      ruri = raw_request_->path_info();
      string qs = raw_request_->query_string();
      if(!qs.empty())
      {
        ruri.push_back('?');
        ruri.append(qs);
      }
    }
    return ruri;
  }

  string content_type() const
  {
    return raw_request_->content_type();
  }

  unsigned long long content_length() const
  {
    return raw_request_->content_length();
  }

  optional<string> get_param(const string& p) const
  {
     auto& get_prms = raw_request_->get();
     auto get_pos = get_prms.find(p);
     if (get_pos != get_prms.end())
       return optional<string>(get_pos->second);

     auto& post_prms = raw_request_->post();
     auto post_pos = post_prms.find(p);
     if (post_pos != post_prms.end())
       return optional<string>(post_pos->second);

     return optional<string>();
  }

  optional<string> get_header(const string& name) const
  {
    if(name.empty())
      return optional<string>();

    const std::locale cloc = std::locale::classic();
    string buff;
    buff.reserve(name.length() + 6);
    buff.append("HTTP_", 5);
    for(const char& c : name)
    {
      buff.push_back(std::isalnum(c, cloc) ? std::toupper(c, cloc) : '_');
    }

    const char* found = raw_request_->cgetenv(buff.c_str());
    if(found)
      return optional<string>(string(found));

    return optional<string>();
  }
};

}}
#endif
