#pragma once

#include <basic_clients/TCPClient.hpp>
#include <models/requests/HttpRequest.hpp>
#include <optional>

namespace httpsli::http {

class HttpClient : public tcp_client::TCPClient {
public:
  HttpClient(
      const std::string &address, int port,
      const tcp_client::ConnectionHandler &connection_handler = std::nullopt);

  void SendRequest(const requests::http::HttpRequest &request,
                   const tcp_client::RecievingHandler &recieving_handler = std::nullopt,
                   const tcp_client::SendingHandler &sending_handler = std::nullopt);

  void Join();
};
} // namespace httpsli::http