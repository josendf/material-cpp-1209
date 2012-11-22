#pragma once
#ifndef HTPARSE_BACKENDS_TESTING_RESULT_HPP_
#define HTPARSE_BACKENDS_TESTING_RESULT_HPP_
#include <string>

namespace htparse { namespace testing {

using std::string;

class result
{
private:
  int code_;

  string msg_;

public:
  result():
    code_(0) {}

  result(int code, const string& msg):
    code_(code),
    msg_(msg) {}

  result(const result& other):
    code_(other.code_),
    msg_(other.msg_) {}

  result(result&& other):
    code_(other.code_),
    msg_(std::move(other.msg_)) {}

  void reset(int code, const string& msg)
  {
    code_ = code_;
    msg_  = msg_;
  }

  int code() const
  {
    return code_;
  }

  const std::string& message() const
  {
    return msg_;
  }

  bool has_error() const
  {
    return code_ >= 400;
  }

  template <class TStm>
  TStm& show(TStm& stm) const
  {
    stm << code_ << ' ' << msg_;
    return stm;
  }
};

}}
#endif
