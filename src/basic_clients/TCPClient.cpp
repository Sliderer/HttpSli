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
       const ConnectionHandler &connection_handler)
      : connection_handler_(connection_handler) {
    ip::tcp::endpoint endpoint(ip::address::from_string(address), port);

    socket_prt_.reset(new ip::tcp::socket(service_));
    socket_prt_->async_connect(endpoint,
                               [this](const boost::system::error_code &error) {
                                 this->ConnectionHandler(error);
                               });
  }

  void SendRequest(const requests::BasicRequest &request,
              const RecievingHandler &recieving_client_handler = std::nullopt,
              const SendingHandler &sending_client_handler = std::nullopt) {
    recieving_client_handler_ = std::move(recieving_client_handler);
    sending_client_handler_ = std::move(sending_client_handler);

    std::shared_ptr<char[]> recieving_buffer(new char[1024]);

    std::string serialized_request = request.Serialize();
    std::shared_ptr<char[]> serialized_request_cstr(
        new char[serialized_request.size() + 1]);
    std::strcpy(serialized_request_cstr.get(), serialized_request.c_str());

    auto sending_handler =
        [this, recieving_buffer](const boost::system::error_code &error,
                                 std::size_t bytes_transferred) {
          auto recieving_handler =
              [this, recieving_buffer](const boost::system::error_code &error,
                                       std::size_t bytes_transferred) {
                if (recieving_client_handler_.has_value()) {
                  (*recieving_client_handler_)(error, bytes_transferred,
                                               recieving_buffer);
                }
              };

          if (sending_client_handler_.has_value()) {
            (*sending_client_handler_)(error, bytes_transferred);
          }

          socket_prt_->async_receive(buffer(recieving_buffer.get(), 1024),
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
  ::httpsli::tcp_client::ConnectionHandler connection_handler_;
  RecievingHandler recieving_client_handler_;
  SendingHandler sending_client_handler_;
};

TCPClient::TCPClient(const std::string &address, int port,
                     const ConnectionHandler &connection_handler) {
  impl_ = std::make_unique<Impl>(address, port, connection_handler);
}

void TCPClient::SendRequest(const requests::BasicRequest &request,
                            const RecievingHandler &recieving_handler,
                            const SendingHandler &sending_handler) {
  impl_->SendRequest(request, recieving_handler, sending_handler);
}

void TCPClient::Disconnect() { impl_->Disconnect(); }

void TCPClient::Join() { impl_->Join(); }

TCPClient::~TCPClient() {}

} // namespace httpsli::tcp_client