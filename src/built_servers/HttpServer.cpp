#include "HttpServer.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <iostream>
#include <memory>
#include <optional>

#include <models/requests/HttpRequest.hpp>

namespace httpsli::http {
HttpServer::HttpServer(const std::string &address, int port,
                       const httpsli::helpers::http::AddressRouter &router,
                       size_t max_reading_buffer_size)
    : httpsli::tcp_server::TCPServer(
          address, port,
          [this](boost::asio::ip::tcp::socket &socket) {
            this->ClientSession(socket);
          }),
      router_(router), max_reading_buffer_size_(max_reading_buffer_size) {}

void HttpServer::ClientSession(boost::asio::ip::tcp::socket &socket) const {
  std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr =
      std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket));

  ReadFromSocket(socket_ptr);
}

void HttpServer::ReadFromSocket(
    const std::shared_ptr<boost::asio::ip::tcp::socket>& socket_ptr) const {

  std::shared_ptr<char[]> buffer(new char[max_reading_buffer_size_]);

  auto recieve_handler = [this, buffer,
                          socket_ptr](const boost::system::error_code &error,
                                      std::size_t bytes_transferred) {
    if (error.failed()) {
      return;
    }

    httpsli::requests::http::HttpRequest request =
        httpsli::requests::http::HttpRequestConstructor::Construct(
            buffer.get());
    std::optional<Handler> handler = FindHandler(request);
    if (!handler.has_value()) {
      return;
    }

    std::shared_ptr<httpsli::responses::http::HttpResponse> response =
        std::make_shared<httpsli::responses::http::HttpResponse>(
            (*handler)(request));

    WriteToSocket(socket_ptr, response);
  };

  socket_ptr->async_receive(boost::asio::buffer(buffer.get(), max_reading_buffer_size_),
                            recieve_handler);
}

void HttpServer::WriteToSocket(
    const std::shared_ptr<boost::asio::ip::tcp::socket>& socket,
    const std::shared_ptr<httpsli::responses::http::HttpResponse>& response) const {

  std::string serialized_response = response->Serialize();
  std::shared_ptr<char[]> serialized_response_cstr(
      new char[serialized_response.size() + 1]);
  std::strcpy(serialized_response_cstr.get(), serialized_response.c_str());

  std::function<void(const boost::system::error_code &, std::size_t)>
      send_handler = [socket](const boost::system::error_code &error,
                              std::size_t bytes_transferred) {
        if (error.failed()) {
          return;
        }
        socket->close();
      };

  socket->async_send(boost::asio::buffer(serialized_response_cstr.get(),
                                         serialized_response.size()),
                     send_handler);
}

std::optional<Handler>
HttpServer::FindHandler(const httpsli::requests::http::HttpRequest &request) const {
  return router_.FindHandler(request);
}

} // namespace httpsli::http