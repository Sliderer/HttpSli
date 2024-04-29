#include "TCPServer.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <iostream>

namespace httpsli::tcp_server {
using namespace boost::asio;
class TCPServer::Impl {

public:
  Impl(std::string &&address, int port) : address_(address), port_(port) {}

  Impl(Impl& impl){}

  void StartServer(std::function<int()> user_handler) {
    StartWaitingForAccept();
  }

  void Join() { service_.run(); }
  ~Impl() {}

private:
  void StartWaitingForAccept(){
    ip::tcp::resolver resolver(service_);
    ip::tcp::socket socket(service_);
    ip::tcp::endpoint endpoint_(ip::tcp::v4(), port_);
    ip::tcp::acceptor acceptor(service_, endpoint_);
    acceptor.async_accept(
        service_, endpoint_, [&](const boost::system::error_code &error,
                    boost::asio::ip::tcp::socket peer) { Accept(peer); });
  }

  void Accept(ip::tcp::socket &peer) {
    std::cout << "Connection accepted";

    StartWaitingForAccept();
  }

private:
  io_service service_;
  std::string address_;
  int port_;
};

TCPServer::TCPServer(std::string &&address, int port) {
  impl_ = std::make_unique<Impl>(std::move(address), port);
}

TCPServer::~TCPServer(){}

} // namespace httpsli::tcp_server