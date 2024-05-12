#pragma once

#include <boost/asio.hpp>

#include <models/requests/BasicRequest.hpp>
#include <memory>
#include <models/responses/BasicResponse.hpp>
#include <type_traits>

namespace httpsli::tcp_client {
using namespace boost::asio;

class TCPClient {
public:
  TCPClient();

  ~TCPClient();

  TCPClient(TCPClient& other) = delete;

  template <
      typename Response, typename Request,
      std::enable_if_t<std::is_base_of_v<requests::BasicRequest, Request>>,
      std::enable_if_t<std::is_base_of_v<responses::BasicResponse, Response>>>
  Response SendRequest(const Request &request);

  void Connect(const std::string& address, int port);

  void Disconnect();

  void Join();
   

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace httpsli::tcp_client