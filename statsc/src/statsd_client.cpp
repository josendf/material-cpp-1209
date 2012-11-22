#include "precompiled.h"
#include <statsc/statsd_client.hpp>

namespace statsd {
using std::string;

client::client():
  enabled_(false),
  rdist_(0.0, 1.0),
  socket_(service_) {}

const char* client::default_port()
{
  return "8125";
}

client::client(const string& host,
               const string& port,
               const string& prefix):
  enabled_(!host.empty() && !port.empty()),
  rdist_(0.0, 1.0),
  socket_(service_),
  host_(host),
  port_(port),
  prefix_(prefix) {}

client::~client()
{
  using boost::system::error_code;
  using boost::asio::ip::udp;
  lock_guard lock(mutex_);
  error_code ec;
  socket_.shutdown(udp::socket::shutdown_both, ec);
  socket_.close(ec);
}

void client::configure(const string& host,
                       const string& port,
                       const string& prefix)
{
  host_ = host;
  port_ = port;
  prefix_ = prefix;
  enabled_ = !host.empty() && !port.empty();
}

client::status client::send(const metric& mt)
{
  using std::stringstream;
  using std::locale;

  if(!enabled_)
    return status::omitted;

  if(!should_send(mt))
    return status::omitted;

  stringstream buf;
  buf.imbue(locale::classic());
  mt.write(prefix_, buf);
  return send_message(buf.str());
}

client::status client::send_message(const std::string& msg)
{
  using boost::asio::buffer;
  using boost::system::error_code;

  if(msg.empty())
    return status::omitted;

  lock_guard lock(mutex_);

  if(!connect())
    return status::failed;

  error_code ec;
  const std::size_t sent =
    socket_.send_to(buffer(msg), remote_endpoint_, 0, ec);
  if(ec || sent < msg.length())
    return status::failed;

  return status::sent;
}

bool client::should_send(const metric& mt)
{
  if(mt.empty())
    return false;
  const double sample_rate = mt.rate();
  return sample_rate < 1.0 ? sample_rate > rdist_(rgen_) : true;
}

bool client::connect()
{
  return resolve_remote_endpoint() && open_socket();
}

bool client::resolve_remote_endpoint()
{
  using boost::asio::ip::udp;
  using boost::system::error_code;

  if(!remote_endpoint_.address().is_unspecified())
    return true;

  error_code ec;
  udp::resolver rslvr(service_);
  udp::resolver::query qry(udp::v4(), host_, port_);
  udp::resolver::iterator it = rslvr.resolve(qry, ec);
  const udp::resolver::iterator end;
  if(ec || it == end)
    return false;

  remote_endpoint_ = *it;

  return true;
}

bool client::open_socket()
{
  using boost::asio::ip::udp;
  using boost::system::error_code;

  if(socket_.is_open())
    return true;

  error_code ec;

  socket_.open(udp::v4(), ec);
  if(ec)
    return false;

  udp::endpoint local_endpoint(udp::v4(), 0);
  socket_.bind(local_endpoint, ec);
  if(ec)
  {
    socket_.close();
    return false;
  }

  return true;
}

}
