#include "TCPClient.hpp"
#include "models/requests/BasicRequest.hpp"
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/asio/placeholders.hpp>
#include <iostream>
#include <memory>

namespace httpsli::tcp_client {
class TCPClient::Impl {
public:
  Impl(const std::string& address, int port){
    ip::tcp::endpoint endpoint(ip::address::from_string(address), port);

    socket_prt_.reset(new ip::tcp::socket(service_));
    socket_prt_->async_connect(endpoint, [this](const boost::system::error_code &error){
        this->ConnectionHandler();
    });
  }

  void SendRequest(const requests::BasicRequest &request) {
    std::cout << "Sending reqeust\n";

    std::shared_ptr<char[]> reading_buffer(new char[1024]);

    std::string serialized_request = request.Serialize();
    std::shared_ptr<char[]> serialized_request_cstr(
        new char[serialized_request.size() + 1]);
    std::strcpy(serialized_request_cstr.get(), serialized_request.c_str());

    std::cout << serialized_request << '\n';

    auto sending_handler =
        [this, reading_buffer](const boost::system::error_code &error,
                               std::size_t bytes_transferred) {
          auto recieving_handler =
              [reading_buffer](const boost::system::error_code &error,
                               std::size_t bytes_transferred) {
                std::cout << "Readed " << bytes_transferred << '\n';
                std::cout << "Got " << reading_buffer << '\n';
              };
          std::cout << "Recieving data\n";
          socket_prt_->async_receive(buffer(reading_buffer.get(), 1024),
                                recieving_handler);
        };

    socket_prt_->async_send(
        buffer(serialized_request_cstr.get(), serialized_request.size()),
        sending_handler);
  }

  void Disconnect() { socket_prt_->close(); }

  void Join() { service_.run(); }

private:
  void ConnectionHandler() {
  }

private:
  io_service service_;
  std::shared_ptr<ip::tcp::socket> socket_prt_;
};

TCPClient::TCPClient(const std::string& address, int port) {
    impl_ = std::make_unique<Impl>(address, port);
}

void TCPClient::SendRequest(const requests::BasicRequest &request) {
  impl_->SendRequest(request);
}

void TCPClient::Disconnect() { impl_->Disconnect(); }

void TCPClient::Join() { impl_->Join(); }

TCPClient::~TCPClient() {}

} // namespace httpsli::tcp_client