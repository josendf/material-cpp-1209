#include "precompiled.h"
#include <statsc/statsd_metric.hpp>

namespace statsd {
using std::string;

metric::metric(tag t, const string& name, double value, double rate):
  start_time_(t == tag::timer ?
              boost::posix_time::microsec_clock::universal_time() :
              boost::posix_time::not_a_date_time),
  tag_(t),
  name_(name),
  value_(value),
  rate_(boost::algorithm::clamp(rate, 0.0, 1.0)){}

metric::metric(const metric& other):
   start_time_(other.start_time_),
   tag_(other.tag_),
   name_(other.name_),
   value_(other.value_),
   rate_(other.rate_) {}

metric::metric(metric&& other):
  start_time_(other.start_time_),
  tag_(other.tag_),
  name_(std::move(other.name_)),
  value_(other.value_),
  rate_(other.rate_) {}

metric& metric::operator =(const metric& other)
{
  if(this != &other)
  {
    start_time_ = other.start_time_;
    tag_ = other.tag_;
    name_ = other.name_;
    value_ = other.value_;
    rate_ = other.rate_;
  }
  return *this;
 }

metric& metric::operator =(metric&& other)
{
  if(this != &other)
  {
    start_time_ = other.start_time_;
    tag_ = other.tag_;
    name_ = std::move(other.name_);
    value_ = other.value_;
    rate_ = other.rate_;
  }
  return *this;
 }

boost::posix_time::time_duration metric::elapsed() const
{
  using boost::posix_time::time_duration;
  return start_time_.is_special() ? time_duration() :
    boost::posix_time::microsec_clock::universal_time() - start_time_;
}

double metric::elapsed_ms() const
{
  using boost::posix_time::time_duration;
  const time_duration elp = elapsed();
  // 1 ms = 1000 us
  const long us = elp.total_microseconds();
  return static_cast<double>(us) * 0.001;
}


}
