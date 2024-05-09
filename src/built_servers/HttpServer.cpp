#include "HttpServer.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <iostream>
#include <memory>
#include <optional>

#include <models/requests/HttpRequest.hpp>

namespace httpsli::http {
HttpServer::HttpServer(std::string &address, int port,
                       httpsli::helpers::http::AddressRouter router)
    : httpsli::tcp_server::TCPServer(address, port,
                                     [this](boost::asio::ip::tcp::socket &socket) {
                                       this->ClientSession(socket);
                                     }),
      router_(router) {}

void HttpServer::ClientSession(boost::asio::ip::tcp::socket &socket) {
  std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr =
      std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket));

  ReadFromSocket(socket_ptr);
}

void HttpServer::ReadFromSocket(
  std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr) {

  std::shared_ptr<char[]> buffer(new char[1024]);

  auto reading_handler = [this, buffer,
                          socket_ptr](const boost::system::error_code &error,
                                      std::size_t bytes_transferred) {
    if (error.failed()) {
      std::cout << "Error: " << error.message() << '\n';
      return;
    }

    std::cout << "Request " << buffer << '\n';

    httpsli::requests::http::HttpRequest request = httpsli::requests::http::HttpRequestConstructor::Construct(buffer.get());
    std::optional<Handler> handler = FindHandler(request);
    if (!handler.has_value()) {
      std::cout << "Can not find handler\n";
      return;
    }

    std::shared_ptr<httpsli::responses::http::HttpResponse> response =
        std::make_shared<httpsli::responses::http::HttpResponse>(
            (*handler)(request));

    WriteToSocket(socket_ptr, response);
  };

  socket_ptr->async_read_some(boost::asio::buffer(buffer.get(), 1024),
                              reading_handler);
}

void HttpServer::WriteToSocket(
    std::shared_ptr<boost::asio::ip::tcp::socket> socket,
    std::shared_ptr<httpsli::responses::http::HttpResponse> response) {

  std::string serialized_response = response->Serialize();
  char *serialized_response_cstr = new char[serialized_response.size()];
  std::strcpy(serialized_response_cstr, serialized_response.c_str());

  std::cout << "Answer: " << serialized_response_cstr << '\n';
  std::function<void(const boost::system::error_code &, std::size_t)>
      write_handler = [socket](const boost::system::error_code &error,
                               std::size_t bytes_transferred) {
        if (error.failed()) {
          std::cout << "Failed to write in socket\n";
          return;
        }
        socket->close();
      };

  std::cout << "Setting answer handler\n";
  socket->async_write_some(boost::asio::buffer(serialized_response_cstr,
                                               serialized_response.size()),
                           write_handler);
}

std::optional<Handler>
HttpServer::FindHandler(httpsli::requests::http::HttpRequest &request) {
  return router_.FindHandler(request);
}

} // namespace httpsli::http