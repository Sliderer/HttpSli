#include "AddressRouter.hpp"
#include <optional>
#include <regex>

namespace httpsli::helpers::http {

HandlerPair::HandlerPair(const std::string &address, const Handler &handler)
    : address_(address), handler_(handler) {}

bool HandlerPair::IsMatching(const std::string &address) const {
  std::regex regex_expression(address_);
  return std::regex_match(address, regex_expression);
}

Handler HandlerPair::GetHandler() const { return handler_; }

AddressRouter::AddressRouter(const std::vector<HandlerPair> &handlers)
    : handlers_(handlers) {}

void AddressRouter::AddHandler(const HandlerPair &handler) {
  handlers_.push_back(handler);
}

std::optional<Handler> AddressRouter::FindHandler(
    const httpsli::requests::http::HttpRequest &request) const {
  for (const HandlerPair &pair : handlers_) {
    if (pair.IsMatching(request.GetAddress())) {
      return pair.GetHandler();
    }
  }
  return std::nullopt;
}
} // namespace httpsli::helpers::http