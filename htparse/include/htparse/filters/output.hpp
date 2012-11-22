/**
 * @file   output.hpp
 * @author jedf
 * 
 * @brief  
 * 
 * 
 */
#pragma once
#ifndef HTPARSE_FILTERS_OUTPUT_HPP_
#define HTPARSE_FILTERS_OUTPUT_HPP_
#include <htparse/filter.hpp>
#include <string>

namespace htparse { namespace filters {

using std::string;

template <class TMsg>
class output:
    public basic_filter<TMsg>
{
public:
  typedef typename basic_filter<TMsg>::message_type message_type;
  typedef typename basic_filter<TMsg>::parser_type parser_type;
  typedef typename basic_filter<TMsg>::filter_func filter_func;
  typedef typename basic_filter<TMsg>::status_type status_type;

private:
  string content_type_;
  string body_;

public:
  output(const output& other):
    content_type_(other.content_type_),
    body_(other.body_) {}

  output(output&& other):
    content_type_(std::move(other.content_type_)),
    body_(std::move(other.body_)) {}

  output(const string& body):
    body_(body) {}

  output(const string& content_type,
               const string& body):
    content_type_(content_type),
    body_(body) {}

  parser_type operator()(const message_type& msg) const
  {
    return parse(msg.output(content_type_, body_));
  }
};

}}
#endif
