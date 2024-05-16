#include "ClientPool.hpp"
#include "basic_clients/TCPClient.hpp"
#include "built_clients/HttpClient.hpp"
#include "models/requests/HttpRequest.hpp"

namespace httpsli::components {

template <typename ClientType,
          std::enable_if<std::is_base_of_v<tcp_client::TCPClient, ClientType>>>
ClientPool::ClientPool(size_t size, const std::string& address, int port) {
    clients_.assign(size, tcp_client::TCPClient(address, port));
    Initialize();
}

ClientPool::ClientPool(const std::vector<tcp_client::TCPClient> &clients)
    : clients_(clients) {
  Initialize();
}

void ClientPool::Initialize() { is_free_.assign(clients_.size(), 0); }

void ClientPool::MakeRequest(const requests::http::HttpRequest& request, const http::ClientRecievingHandler& handler) const {
    current_client_index_ = (current_client_index_ + 1) % clients_.size();
    if (is_free_[current_client_index_]){
        
    }
}

void ClientPool::Join() const {
  for (const auto &client : clients_) {
    client.Join();
  }
}

} // namespace httpsli::components