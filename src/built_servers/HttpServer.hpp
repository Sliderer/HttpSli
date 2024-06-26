#pragma once

#include <boost/asio.hpp>

#include <basic_servers/TCPServer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <helpers/http/AddressRouter.hpp>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>

#include <functional>
#include <sstream>

namespace httpsli::http {

using Handler = std::function<httpsli::responses::http::HttpResponse(
    httpsli::requests::http::HttpRequest &)>;

class HttpServer : public httpsli::tcp_server::TCPServer {
public:
  HttpServer(const std::string &address, int port,
             const httpsli::helpers::http::AddressRouter& router, 
             size_t max_reading_buffer_size = 16384);

private:
  void ClientSession(boost::asio::ip::tcp::socket &socket) const;

  void ReadFromSocket(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket) const;

  void WriteToSocket(
      const std::shared_ptr<boost::asio::ip::tcp::socket>& socket,
      const std::shared_ptr<httpsli::responses::http::HttpResponse>& response) const;

  std::optional<Handler>
  FindHandler(const httpsli::requests::http::HttpRequest &request) const;

private:
  httpsli::helpers::http::AddressRouter router_;
  size_t max_reading_buffer_size_;
};
} // namespace httpsli::http