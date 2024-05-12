#include "TCPServer.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <functional>
#include <iostream>
#include <memory>

namespace httpsli::tcp_server {
using namespace boost::asio;

class TCPServer::Impl {
public:
  Impl(const std::string &address, int port, const ClientSession& client_session)
      : address_(address), port_(port), client_session_(client_session) {}

  Impl(Impl &impl) {}

  void Join() {}

  void StartServer() {
    ip::tcp::acceptor acceptor = InitializeServer();
    StartWaitingForAccept(acceptor);
    service_.run();
  }

  ~Impl() {}

private:
  ip::tcp::acceptor InitializeServer() {
    ip::tcp::resolver resolver(service_);

    endpoint_ = ip::tcp::endpoint(ip::tcp::v4(), port_);
    return ip::tcp::acceptor(service_, endpoint_);
  }

  void StartWaitingForAccept(ip::tcp::acceptor &acceptor) {
    acceptor.async_accept(service_, endpoint_,
                          [&](const boost::system::error_code &error,
                              boost::asio::ip::tcp::socket peer) {
                            // TODO: add fiber here

                            Accept(peer, acceptor);
                          });
  }

  void Accept(ip::tcp::socket &peer, ip::tcp::acceptor &acceptor) {

    client_session_(peer);

    StartWaitingForAccept(acceptor);
  }

private:
  io_service service_;
  std::string address_;
  ip::tcp::endpoint endpoint_;
  int port_;
  ClientSession client_session_;
};

TCPServer::TCPServer(const std::string &address, int port,
                     const ClientSession& client_session) {
  impl_ = std::make_unique<Impl>(address, port, client_session);
}

void TCPServer::StartServer() const {
  impl_->StartServer();
}

void TCPServer::Join() const { impl_->Join(); }

TCPServer::~TCPServer() {}

} // namespace httpsli::tcp_server