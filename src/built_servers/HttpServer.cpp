#include "HttpServer.hpp"

#include <boost/asio/buffer.hpp>
#include <iostream>
#include <optional>

namespace httpsli::http {
HttpServer::HttpServer(std::string &address, int port,
                       httpsli::helpers::http::AddressRouter router)
    : httpsli::tcp_server::TCPServer(address, port,
                                     [&](boost::asio::ip::tcp::socket &socket) {
                                       this->ClientSession(socket);
                                     }),
      router_(router) {}

void HttpServer::ClientSession(boost::asio::ip::tcp::socket &socket) {
  std::stringstream data_stream;
  ReadFromSocket(socket, 1024, data_stream);
}

void HttpServer::ReadFromSocket(boost::asio::ip::tcp::socket &socket,
                                int read_size, std::stringstream &data) {
  std::string current_part;

  std::function<void(const boost::system::error_code &, std::size_t)>
      reading_handler = [&](const boost::system::error_code &error,
                            std::size_t bytes_transferred) {
        if (bytes_transferred != 0 && bytes_transferred == read_size) {
          socket.async_read_some(boost::asio::buffer(current_part),
                                 reading_handler);
        }
      };

  std::function<void(const boost::system::error_code &, std::size_t)>
      initial_reading_handler = [&](const boost::system::error_code &error,
                                    std::size_t bytes_transferred) {
        if (bytes_transferred != 0 && bytes_transferred == read_size) {

          socket.async_read_some(boost::asio::buffer(current_part),
                                 reading_handler);
        }

        httpsli::requests::http::HttpRequest request;
        std::optional<Handler> handler = FindHandler(request);
        if (!handler.has_value()) {
          std::cout << "Can not find handler\n";
          return;
        }

        httpsli::responses::http::HttpResponse response = (*handler)(request);

        WriteToScoket(socket, 1024, response);
      };

  socket.async_read_some(boost::asio::buffer(current_part),
                         initial_reading_handler);
}

void HttpServer::WriteToScoket(
    boost::asio::ip::tcp::socket &socket, int write_size,
    httpsli::responses::http::HttpResponse &response) {
  std::string serialized_response = response.Serialize();

  std::function<void(const boost::system::error_code &, std::size_t)>
      write_handler = [&](const boost::system::error_code &error,
                          std::size_t bytes_transferred) {
        if (bytes_transferred != 0 && bytes_transferred == write_size) {
          serialized_response = serialized_response.substr(1024);
          socket.async_write_some(boost::asio::buffer(serialized_response),
                                  write_handler);
        }
      };

  socket.async_write_some(boost::asio::buffer(serialized_response), write_handler);
}

std::optional<Handler>
HttpServer::FindHandler(httpsli::requests::http::HttpRequest &request) {
  return router_.FindHandler(request);
}

} // namespace httpsli::http