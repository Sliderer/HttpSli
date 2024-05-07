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
using ClientSession =
    std::function<responses::http::HttpResponse(requests::http::HttpRequest &)>;

class HttpServer : public httpsli::tcp_server::TCPServer {
public:
  HttpServer(std::string &address, int port,
             httpsli::helpers::http::AddressRouter router);

private:
  void ClientSession(boost::asio::ip::tcp::socket &socket,
                     boost::asio::io_service &service, 
                     std::string& buffer);

  void ReadFromSocket(boost::asio::ip::tcp::socket &socket,
                      boost::asio::io_service &service, std::string buffer);

  void WriteToSocket(boost::asio::ip::tcp::socket* socket,
                     httpsli::responses::http::HttpResponse* response);

  std::optional<Handler>
  FindHandler(httpsli::requests::http::HttpRequest &request);

private:
  httpsli::helpers::http::AddressRouter router_;
};
} // namespace httpsli::http