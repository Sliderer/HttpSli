#include "TCPServer.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <functional>
#include <iostream>
#include <thread>

namespace httpsli::tcp_server {
using namespace boost::asio;

class TCPServer::Impl {

public:
  Impl(std::string &address, int port, ClientSession client_session)
      : address_(address), port_(port), client_session_(client_session) {}

  Impl(Impl &impl) {}

  void StartServer() {
    ip::tcp::acceptor acceptor = InitializeServer();
    StartWaitingForAccept(acceptor);
  }

  ~Impl() {}

private:
  ip::tcp::acceptor InitializeServer() {
    std::cout << "Initializing server\n";
    ip::tcp::resolver resolver(service_);

    endpoint_ = ip::tcp::endpoint(ip::tcp::v4(), port_);
    return ip::tcp::acceptor(service_, endpoint_);
  }

  void StartWaitingForAccept(ip::tcp::acceptor &acceptor) {
    std::cout << "Waiting for clients\n";
    ip::tcp::socket socket(service_);
    acceptor.async_accept(service_, endpoint_,
                          [&](const boost::system::error_code &error,
                              boost::asio::ip::tcp::socket peer) {
                            // TODO: add fiber here
                            io_service current_service;
                            std::string buffer(1024, ' ');
                            Accept(peer, acceptor, current_service, buffer);
                            current_service.run();
                          });
    service_.run();
    std::cout << "end of waiting";
  }

  void Accept(ip::tcp::socket &peer, ip::tcp::acceptor &acceptor,
              io_service &service, std::string &buffer) {
    std::cout << "Connection accepted\n";

    client_session_(peer, service, buffer);

    StartWaitingForAccept(acceptor);
  }

private:
  io_service service_;
  std::string address_;
  ip::tcp::endpoint endpoint_;
  int port_;
  ClientSession client_session_;
};

TCPServer::TCPServer(std::string &address, int port,
                     ClientSession client_session) {
  impl_ = std::make_unique<Impl>(address, port, client_session);
}

void TCPServer::StartServer() {
  std::cout << "Starting server\n";
  impl_->StartServer();
}

TCPServer::~TCPServer() {}

} // namespace httpsli::tcp_server