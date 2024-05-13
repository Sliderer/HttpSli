#pragma once

#include <boost/asio.hpp>

#include <models/requests/BasicRequest.hpp>
#include <memory>
#include <models/responses/BasicResponse.hpp>

namespace httpsli::tcp_client {
using namespace boost::asio;

class TCPClient {
public:
  TCPClient(const std::string& address, int port);

  ~TCPClient();

  TCPClient(TCPClient& other) = delete;

  void SendRequest(const requests::BasicRequest &request);

  void Disconnect();

  void Join();
   

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace httpsli::tcp_client