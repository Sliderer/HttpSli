#pragma once

#include <boost/asio.hpp>

#include <basic_clients/TCPClient.hpp>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>
#include <optional>

namespace httpsli::http {

using ClientRecievingHandler = std::optional<
    std::function<void(const responses::http::HttpResponse&)>>;

class HttpClient : public tcp_client::TCPClient {
public:
  HttpClient(
      const std::string &address, int port,
      const tcp_client::ConnectionHandler &connection_handler = std::nullopt);

  void SendRequest(const requests::http::HttpRequest &request,
                   const ClientRecievingHandler &recieving_handler = std::nullopt,
                   const tcp_client::SendingHandler &sending_handler = std::nullopt);

  void Join();

private:
  void RecievingHandler(const boost::system::error_code & error_code, std::size_t bytes_transferred,
                       char buffer[], ClientRecievingHandler handler);
};
} // namespace httpsli::http