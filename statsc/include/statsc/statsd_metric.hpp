/**
 * @file   statsd_client.hpp
 * @author jedf
 *
 * @brief  Simple statsd stats aggregator service client.
 * See https://github.com/etsy/statsd.
 */
#pragma once
#ifndef STATSC_STATSD_METRIC_HPP_
#define STATSC_STATSD_METRIC_HPP_
#include <string>
#include <ext/boost/date_time.hpp>
#include <ext/boost/algorithm.hpp>

namespace statsd {
using std::string;

/** A metric value to be sent to the statsd stats aggregator service.
 * See https://github.com/etsy/statsd.
 */
class metric
{
public:
  enum class tag
  {
    empty,
    counter,
    timer,
    gauge,
    set
  };

private:
  boost::posix_time::ptime start_time_;
  tag tag_;
  string name_;
  double value_;
  double rate_;

public:
  /** Default constructor of #metric.
   *
   */
  metric():
    tag_(tag::empty),
    value_(0.0),
    rate_(1.0) {}

  /** Constructs an instance of #metric.
   *
   * @param[in] t The metric type.
   * @param[in] name The metric name.
   * @param[in] value The metric value.
   * @param[in] rate The sampling rate.
   */
  metric(tag t, const string& name, double value, double rate);

  static inline metric start_timer(const string& name)
  {
    return metric(tag::timer, name, 0.0, 1.0);
  }

  static inline metric sum_counter(const string& name,
                                   const double value,
                                   const double rate = 1.0)
  {
    return metric(tag::counter, name, value, rate);
  }

  static inline metric inc_counter(const string& name, const double rate = 1.0)
  {
    return metric(tag::counter, name, 1.0, rate);
  }

  static inline metric dec_counter(const string& name, const double rate = 1.0)
  {
    return metric(tag::counter, name, -1.0, rate);
  }

  static inline metric set_gauge(const string& name, const double value)
  {
    return metric(tag::gauge, name, value, 1.0);
  }

  static inline metric unique(const string& name, const int value)
  {
    return metric(tag::set, name, static_cast<double>(value), 1.0);
  }

  /** Copy constructor of #metric.
   *
   * @param[in] other An existing instance.
   */
  metric(const metric& other);

  /** Move constructor of #metric.
   *
   * @param[in] other An existing instance.
   */
  metric(metric&& other);

  /** Copy assignment of #metric.
   *
   * @param[in] other An existing instance.
   */
  metric& operator =(const metric& other);

  /** Move assignment of #metric.
   *
   * @param[in] other An existing instance.
   */
  metric& operator =(metric&& other);

  /** The UTC time when this instance was created.
   */
  boost::posix_time::ptime start_time() const
  {
    return start_time_;
  }

  /** The elpased time after this instance was created.
   */
  boost::posix_time::time_duration elapsed() const;

  /** The elpased milliseconds after this instance was created.
   */
  double elapsed_ms() const;

  /** The metric value kind.
   */
  tag kind() const
  {
    return tag_;
  }

  /** Whether this instance is empty initialized.
   */
  bool empty() const
  {
    return tag_ == tag::empty;
  }

  /** The metric name.
   */
  const string& name() const
  {
    return name_;
  }

  /** The metric value.
   */
  double value() const
  {
    return value_;
  }

  /** The metric sampling rate.
   */
  double rate() const
  {
    return rate_;
  }

  /** Writes the corresponding statsd protocol message to the
   * specified stream.
   */
  template<class TStm>
  TStm& write(const string& prefix, TStm& stm) const
  {
    if(!prefix.empty())
    {
      stm << prefix << '.';
    }

    if(tag_ == tag::counter)
    {
      stm << name_ << ':' << value_ << '|' << 'c';
      if(rate_ < 1.0)
        stm << '@' << rate_;
    }
    else if(tag_ == tag::timer)
    {
      const double ms = elapsed_ms();
      stm << name_ << ':' << ms << '|' << 'm' << 's';
    }
    else if(tag_ == tag::gauge)
    {
      stm << name_ << ':' << value_<< '|' << 'g';
    }
    else if(tag_ == tag::set)
    {
      stm << name_ << ':' << value_<< '|' << 's';
    }
    return stm;
  }
};

}
#endif
