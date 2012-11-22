/**
 * @file   statsd_client.hpp
 * @author jedf
 *
 * @brief  Simple statsd stats aggregator service client.
 * See https://github.com/etsy/statsd.
 */
#pragma once
#ifndef STATSC_STATSD_CLIENT_HPP_
#define STATSC_STATSD_CLIENT_HPP_
#include <string>
#include <sstream>
#include <iomanip>
#include <locale>
#include <ext/boost.hpp>
#include <ext/boost/asio.hpp>
#include <ext/boost/thread.hpp>
#include <ext/boost/random.hpp>
#include <statsc/statsd_metric.hpp>

namespace statsd {
using std::string;

/** Simple statsd stats aggregator service client.
 * See https://github.com/etsy/statsd.
 */
class client: boost::noncopyable
{
public:
  enum class status
  {
    sent,
    omitted,
    failed
  };

private:
  typedef boost::mutex mutex_type;
  typedef boost::lock_guard<boost::mutex> lock_guard;

  bool enabled_;
  mutex_type mutex_;
  boost::random::mt11213b rgen_;
  boost::random::uniform_real_distribution<double> rdist_;
  boost::asio::io_service service_;
  boost::asio::ip::udp::endpoint remote_endpoint_;
  boost::asio::ip::udp::socket socket_;
  string host_;
  string port_;
  string prefix_;

public:

  /** Default constructor of #client.
   *
   */
  client();

  /** Constructs an instance of #client.
   *
   * @param[in] host The statsd service host.
   * @param[in] port The statsd service port.
   * @param[in] prefix The metric name prefix.
   */
  client(const string& host,
         const string& port,
         const string& prefix);

  ~client();

  /** The statsd service default port (8125)
   */
  static const char* default_port();

  /** Whether metrics are sent to the aggregator service or not.
   */
  bool enabled() const
  {
    return enabled_;
  }

  /** Whether metrics are sent to the aggregator service or not.
   */
  bool& enabled()
  {
    return enabled_;
  }

  /** The statsd service host.
   */
  const string& host() const
  {
    return host_;
  }

  /** The statsd service port.
   */
  const string& port() const
  {
    return port_;
  }

  /** The metrics name prefix.
   */
  const string& prefix() const
  {
    return prefix_;
  }

  /** Configures the aggregator service address using the
   * default port number (8125).
   *
   * @param[in] host The statsd service host.
   */
  void configure(const string& host)
  {
    const string port(default_port());
    const string prefix;
    configure(host, port, prefix);
  }

  /** Configures the aggregator service address.
   *
   * @param[in] host The statsd service host.
   * @param[in] port The statsd service port.
   * @param[in] prefix The metric name prefix.
   */
  void configure(const string& host,
                 const string& port,
                 const string& prefix);

  /** Send the specified metric value to the stats aggregator service.
   *
   * @param[in] mt The metric value.
   */
  status send(const metric& mt);

  /** Send the specified metric values to the stats aggregator service
   * in a single message.
   *
   * @param[in] mts The metric values.
   */
  template<class TSeq>
  status send_batch(const TSeq& mts)
  {
    using std::stringstream;
    using std::locale;

    if(!enabled_)
      return status::omitted;

    stringstream buf;
    buf.imbue(locale::classic());

    const auto b = std::begin(mts);
    const auto e = std::end(mts);
    for(auto i = b; i != e; ++i)
    {
      const metric& mt = *i;
      if(should_send(mt))
      {
        if(i != b)
          buf << '\n';
        mt.write(prefix_, buf);
      }
    }

    return send_message(buf.str());
  }

private:
  /** Send the specified message to the stats aggregator service.
   *
   * @param[in] msg The message to send.
   * @return The operation status.
   */
  status send_message(const std::string& msg);

  /** Determines if a sample value should be sent to the aggregator service.
   *
   * @param[in] mt The metric value.
   * @return true if the value should be sent.
   */
  bool should_send(const metric& mt);

  /** Resolves the remote endpoint address and opens and binds the UPD socket.
   *
   * @return true if the operations succeeded.
   */
  bool connect();

  /** Resolves the remote endpoint address.
   *
   * @return true if the address was successfully resolved.
   */
  bool resolve_remote_endpoint();

  /** Opens and binds the UPD socket.
   *
   * @return true if the socket was successfully opened.
   */
  bool open_socket();

};

}
#endif
