#pragma once

#include <boost/asio.hpp>

#include <memory>
#include <models/requests/BasicRequest.hpp>
#include <models/responses/BasicResponse.hpp>
#include <optional>

namespace httpsli::tcp_client {

using ClientHandler =
    std::optional<std::function<void(const boost::system::error_code &error)>>;

using namespace boost::asio;

class TCPClient {
public:
  TCPClient(const std::string &address, int port,
            ClientHandler connection_handler, ClientHandler sending_handler,
            ClientHandler recieving_handler);

  ~TCPClient();

  TCPClient(TCPClient &other) = delete;

  void SendRequest(const requests::BasicRequest &request);

  void Disconnect();

  void Join();

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace httpsli::tcp_client