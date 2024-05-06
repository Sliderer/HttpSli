#include "HttpServer.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <iostream>
#include <optional>

namespace httpsli::http {
HttpServer::HttpServer(std::string &address, int port,
                       httpsli::helpers::http::AddressRouter router)
    : httpsli::tcp_server::TCPServer(address, port,
                                     [&](boost::asio::ip::tcp::socket &socket,
                                         boost::asio::io_service &service, std::string& buffer) {
                                       this->ClientSession(socket, service, buffer);
                                     }),
      router_(router) {}

void HttpServer::ClientSession(boost::asio::ip::tcp::socket &socket,
                               boost::asio::io_service &service, std::string& buffer) {
  ReadFromSocket(socket, service, buffer);
}

void HttpServer::ReadFromSocket(boost::asio::ip::tcp::socket &socket,
                                boost::asio::io_service &service, std::string& buffer) {

  std::function<void(const boost::system::error_code &, std::size_t)>
      reading_handler = [&](const boost::system::error_code &error,
                            std::size_t bytes_transferred) {
        std::cout << error << '\n';
        std::cout << "Readed " << bytes_transferred << '\n';
        std::cout << "Request " << buffer << '\n';
        httpsli::requests::http::HttpRequest request;
        std::optional<Handler> handler = FindHandler(request);
        if (!handler.has_value()) {
          std::cout << "Can not find handler\n";
          return;
        }

        httpsli::responses::http::HttpResponse response = (*handler)(request);

        WriteToScoket(socket, response);
      };

      
  socket.async_read_some(boost::asio::buffer(buffer, buffer.size()), reading_handler);
}

void HttpServer::WriteToScoket(
    boost::asio::ip::tcp::socket &socket,
    httpsli::responses::http::HttpResponse &response) {

  std::string serialized_response = response.Serialize();

  std::function<void(const boost::system::error_code &, std::size_t)>
      write_handler = [&](const boost::system::error_code &error,
                          std::size_t bytes_transferred) { socket.close(); };

  socket.async_write_some(boost::asio::buffer(serialized_response),
                          write_handler);
}

std::optional<Handler>
HttpServer::FindHandler(httpsli::requests::http::HttpRequest &request) {
  return router_.FindHandler(request);
}

} // namespace httpsli::http