#include <boost/asio/ip/tcp.hpp>
#include <models/requests/BasicRequest.hpp>
#include <models/responses/BasicResponse.hpp>

#include <functional>
#include <memory>
#include <string>

#include <boost/asio.hpp>

namespace httpsli::tcp_server {
using namespace httpsli::responses;
using namespace httpsli::requests;

using ClientSession = std::function<void(boost::asio::ip::tcp::socket &)>;

class TCPServer {
public:
  TCPServer(const std::string &address, int port, const ClientSession& client_session);

  void StartServer() const;

  void Join() const;

  ~TCPServer();

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace httpsli::tcp_server
