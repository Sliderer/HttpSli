#include "HttpClient.hpp"
#include "models/responses/HttpResponse.hpp"
#include <basic_clients/TCPClient.hpp>
#include <models/requests/HttpRequest.hpp>

#include <iostream>

namespace httpsli::http {
HttpClient::HttpClient(const std::string &address, int port,
                       const tcp_client::ConnectionHandler &connection_handler)
    : tcp_client::TCPClient(address, port, connection_handler) {}

void HttpClient::SendRequest(
    const requests::http::HttpRequest &request,
    const ClientRecievingHandler &recieving_handler,
    const tcp_client::SendingHandler &sending_handler) {
  auto tcp_recieving_handler = [this, recieving_handler](const boost::system::error_code &error_code,
                                   std::size_t bytes_transferred,
                                   char buffer[]) { 

    this->RecievingHandler(error_code, bytes_transferred, buffer,
                           recieving_handler);
  };

  tcp_client::TCPClient::SendRequest(request, tcp_recieving_handler,
                                     sending_handler);
}

void HttpClient::Join() { tcp_client::TCPClient::Join(); }

void HttpClient::RecievingHandler(const boost::system::error_code &error_code,
                                  std::size_t bytes_transferred, char buffer[],
                                  ClientRecievingHandler handler) {
  if (handler.has_value()) {
    std::string response_string(buffer, bytes_transferred);

    std::cout << "Bytes " << bytes_transferred << '\n';

    std::cout << response_string << '\n';

    responses::http::HttpResponse response =
        responses::http::HttpResponseConstructor::Construct(response_string);
    (*handler)(response);
  }
}
} // namespace httpsli::http