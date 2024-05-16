#pragma once

#include <boost/asio.hpp>

#include <memory>
#include <models/requests/BasicRequest.hpp>
#include <models/responses/BasicResponse.hpp>
#include <optional>

namespace httpsli::tcp_client {

using ConnectionHandler =
    std::optional<std::function<void(const boost::system::error_code &error)>>;

using RecievingHandler = std::optional<std::function<void(
    const boost::system::error_code &, std::size_t, char[])>>;

using SendingHandler = std::optional<
    std::function<void(const boost::system::error_code &, std::size_t)>>;

using namespace boost::asio;

class TCPClient {
public:
  TCPClient(const std::string &address, int port,
            const ConnectionHandler &connection_handler = std::nullopt,
            size_t max_reading_buffer_size = 16384);

  ~TCPClient();

  TCPClient(TCPClient &) = delete;
  
  void SendRequest(const requests::BasicRequest &request,
                   const RecievingHandler &recieving_handler = std::nullopt,
                   const SendingHandler &sending_handler = std::nullopt);

  void Disconnect();

  void Join() const;

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace httpsli::tcp_client