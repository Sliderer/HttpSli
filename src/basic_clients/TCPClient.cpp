#include "TCPClient.hpp"
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
  template <
      typename Response, typename Request,
      std::enable_if_t<std::is_base_of_v<requests::BasicRequest, Request>>,
      std::enable_if_t<std::is_base_of_v<responses::BasicResponse, Response>>>
  Response SendRequest(const Request &request) {

    std::shared_ptr<char[]> reading_buffer(new char[1024]);

    std::string serialized_request = request.Serialize();
    std::shared_ptr<char[]> serialized_request_cstr(
        new char[serialized_request.size() + 1]);
    std::strcpy(serialized_request_cstr.get(), serialized_request.c_str());

    auto sending_handler =
        [this, reading_buffer](const boost::system::error_code &error,
                               std::size_t bytes_transferred) {
          auto recieving_handler = [reading_buffer](const boost::system::error_code &error,
                                      std::size_t bytes_transferred) {
            std::cout << "Got " << reading_buffer << '\n';
          };

          socket_.async_receive(buffer(reading_buffer.get(), 1024),
                                recieving_handler);
        };

    socket_.async_send(
        buffer(serialized_request_cstr.get(), serialized_request.size()),
        sending_handler);
  }

  void Connect(const std::string &address, int port) {
    ip::tcp::endpoint endpoint(ip::address::from_string(address), port);

    socket_ = ip::tcp::socket(service_);
    socket_.async_connect(endpoint, &Impl::ConnectionHandler);
  }

  void Disconnect() { socket_.close(); }

  void Join() { service_.run(); }

private:
  void ConnectionHandler(const boost::system::error_code &error) {
    if (error.failed()) {
      std::cout << "Can not connect to server";
    }
  }

private:
  io_service service_;
  ip::tcp::socket socket_;
};

TCPClient::TCPClient() {}

  template <
      typename Response, typename Request,
      std::enable_if_t<std::is_base_of_v<requests::BasicRequest, Request>>,
      std::enable_if_t<std::is_base_of_v<responses::BasicResponse, Response>>>
  Response TCPClient::SendRequest(const Request &request){
    impl_->SendRequest<Response, Request>(request);
  }

void TCPClient::Connect(const std::string& address, int port){
  impl_->Connect(address, port);
}

void TCPClient::Disconnect(){
  impl_->Disconnect();
}

void TCPClient::Join(){
  impl_->Join();
}

TCPClient::~TCPClient() {}

} // namespace httpsli::tcp_client