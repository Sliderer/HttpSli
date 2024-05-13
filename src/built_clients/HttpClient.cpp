#include "HttpClient.hpp"
#include "basic_clients/TCPClient.hpp"
#include "models/requests/HttpRequest.hpp"
#include <optional>

namespace httpsli::http {
HttpClient::HttpClient(const std::string &address, int port,
                       const tcp_client::ConnectionHandler &connection_handler)
    : tcp_client::TCPClient(address, port, connection_handler) {}

void HttpClient::SendRequest(
    const requests::http::HttpRequest &request,
    const tcp_client::RecievingHandler &recieving_handler) {
  tcp_client::TCPClient::SendRequest(request, recieving_handler);
}

void HttpClient::Join() { tcp_client::TCPClient::Join(); }
} // namespace httpsli::http