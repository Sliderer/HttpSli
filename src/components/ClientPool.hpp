#pragma once

#include "basic_clients/TCPClient.hpp"
#include "built_clients/HttpClient.hpp"
#include "models/requests/HttpRequest.hpp"
#include <cstddef>
#include <type_traits>
#include <vector>

namespace httpsli::components {

class ClientPool {
public:
  template <
      typename ClientType,
      std::enable_if<std::is_base_of_v<tcp_client::TCPClient, ClientType>>>
  ClientPool(size_t size, const std::string& address, int port);

  ClientPool(const std::vector<tcp_client::TCPClient> &clients);

  ClientPool(ClientPool &) = delete;

  void MakeRequest(const requests::http::HttpRequest &request,
                   const httpsli::http::ClientRecievingHandler &handler) const;

  void Join() const;

private:
  void Initialize();

private:
  std::vector<tcp_client::TCPClient> clients_;
  int current_client_index_;
  std::vector<bool> is_free_;
};
} // namespace httpsli::components