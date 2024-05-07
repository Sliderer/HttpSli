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
                                boost::asio::io_service &service, std::string) {
  char * buffer = new char[1024];
   
  
  auto reading_handler = [&, buffer](const boost::system::error_code &error,
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

        httpsli::responses::http::HttpResponse* response = new httpsli::responses::http::HttpResponse((*handler)(request));
        delete[] buffer;

        WriteToSocket(socket, response);
      };

      
  socket.async_read_some(boost::asio::buffer(buffer, 1024), reading_handler);
}

void HttpServer::WriteToSocket(
    boost::asio::ip::tcp::socket &socket,
    httpsli::responses::http::HttpResponse* response) {
    
  std::cout << "Answering\n";

  // char* serialized_response_cstr = new char[1024] {"dd"};
  // //std::strcpy(serialized_response_cstr, serialized_response.c_str());

  // std::function<void(const boost::system::error_code &, std::size_t)>
  //     write_handler = [](const boost::system::error_code &error,
  //                         std::size_t bytes_transferred) {};
  // std::cout << "Setting answer handler\n";
  // socket.async_write_some(boost::asio::buffer(serialized_response_cstr, 1024),
  //                         write_handler);

  socket.write_some(boost::asio::buffer("111", 3));
}

std::optional<Handler>
HttpServer::FindHandler(httpsli::requests::http::HttpRequest &request) {
  return router_.FindHandler(request);
}

} // namespace httpsli::http