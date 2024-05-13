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
             const httpsli::helpers::http::AddressRouter& router);

private:
  void ClientSession(boost::asio::ip::tcp::socket &socket) const;

  void ReadFromSocket(std::shared_ptr<boost::asio::ip::tcp::socket> socket) const;

  void WriteToSocket(
      std::shared_ptr<boost::asio::ip::tcp::socket> socket,
      std::shared_ptr<httpsli::responses::http::HttpResponse> response) const;

  std::optional<Handler>
  FindHandler(httpsli::requests::http::HttpRequest &request) const;

private:
  httpsli::helpers::http::AddressRouter router_;
};
} // namespace httpsli::http