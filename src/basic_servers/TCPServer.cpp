#include "TCPServer.hpp"
#include <boost/asio/ip/tcp.hpp>
#include <functional>
#include <iostream>

namespace httpsli::tcp_server {
using namespace boost::asio;

class TCPServer::Impl {

public:
  Impl(std::string& address, int port) : address_(address), port_(port) {}

  Impl(Impl& impl){}

  void StartServer() {
    ip::tcp::acceptor acceptor = InitializeServer();
    StartWaitingForAccept(acceptor);
  }

  void SetClientSession(std::function<void (ip::tcp::socket&)> client_session){
    client_session_ = client_session;
  }

  ~Impl() { }

private:

  ip::tcp::acceptor InitializeServer(){
    std::cout << "Initializing server\n";
    ip::tcp::resolver resolver(service_);
    ip::tcp::socket socket(service_);
    endpoint_ = ip::tcp::endpoint(ip::tcp::v4(), port_);
    return ip::tcp::acceptor(service_, endpoint_);
  }

  void StartWaitingForAccept(ip::tcp::acceptor& acceptor){
    std::cout << "Waiting for clients\n";

    acceptor.async_accept(
        service_, endpoint_, [&](const boost::system::error_code &error,
                    boost::asio::ip::tcp::socket peer) { 
                      //TODO: add fiber here
                      Accept(peer, acceptor);
                    });
    service_.run();
  }

  void Accept(ip::tcp::socket &peer, ip::tcp::acceptor& acceptor) {
    std::cout << "Connection accepted\n";
    
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

TCPServer::TCPServer(std::string& address, int port) {
  impl_ = std::make_unique<Impl>(address, port);
}

void TCPServer::StartServer() {
  std::cout << "Starting server\n";
  impl_->StartServer();
}

void TCPServer::SetClientSession(std::function<void (ip::tcp::socket&)> client_session){
  impl_->SetClientSession(client_session);
}

TCPServer::~TCPServer(){}

} // namespace httpsli::tcp_server