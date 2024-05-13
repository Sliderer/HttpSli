#include "TCPClient.hpp"
#include "models/requests/BasicRequest.hpp"
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>

#include <functional>
#include <memory>
#include <optional>

namespace httpsli::tcp_client {


class TCPClient::Impl {
public:
  Impl(const std::string &address, int port,
       ClientHandler connection_handler = std::nullopt,
       ClientHandler sending_handler = std::nullopt,
       ClientHandler recieving_handler = std::nullopt)
      : connection_handler_(connection_handler),
        sending_handler_(sending_handler),
        recieving_handler_(recieving_handler) {
    ip::tcp::endpoint endpoint(ip::address::from_string(address), port);

    socket_prt_.reset(new ip::tcp::socket(service_));
    socket_prt_->async_connect(endpoint,
                               [this](const boost::system::error_code &error) {
                                 this->ConnectionHandler(error);
                               });
  }

  void SendRequest(const requests::BasicRequest &request) {

    std::shared_ptr<char[]> reading_buffer(new char[1024]);

    std::string serialized_request = request.Serialize();
    std::shared_ptr<char[]> serialized_request_cstr(
        new char[serialized_request.size() + 1]);
    std::strcpy(serialized_request_cstr.get(), serialized_request.c_str());

    auto sending_handler =
        [this, reading_buffer](const boost::system::error_code &error,
                               std::size_t bytes_transferred) {
          auto recieving_handler =
              [this, reading_buffer](const boost::system::error_code &error,
                                     std::size_t bytes_transferred) {
                if (recieving_handler_.has_value()) {
                  (*recieving_handler_)(error);
                }
              };

          if (sending_handler_.has_value()) {
            (*sending_handler_)(error);
          }

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
  void ConnectionHandler(const boost::system::error_code &error) {
    if (connection_handler_.has_value()) {
      (*connection_handler_)(error);
    }
  }

private:
  io_service service_;
  std::shared_ptr<ip::tcp::socket> socket_prt_;
  ClientHandler connection_handler_;
  ClientHandler sending_handler_;
  ClientHandler recieving_handler_;
};

TCPClient::TCPClient(const std::string &address, int port,
                     ClientHandler connection_handler = std::nullopt,
                     ClientHandler sending_handler = std::nullopt,
                     ClientHandler recieving_handler = std::nullopt) {
  impl_ = std::make_unique<Impl>(address, port, connection_handler, sending_handler, recieving_handler);
}

void TCPClient::SendRequest(const requests::BasicRequest &request) {
  impl_->SendRequest(request);
}

void TCPClient::Disconnect() { impl_->Disconnect(); }

void TCPClient::Join() { impl_->Join(); }

TCPClient::~TCPClient() {}

} // namespace httpsli::tcp_client