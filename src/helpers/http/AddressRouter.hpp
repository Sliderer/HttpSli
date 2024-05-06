#pragma once

#include <functional>
#include <optional>
#include <regex>
#include <vector>

#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>

namespace httpsli::helpers::http {

using Handler = std::function<httpsli::responses::http::HttpResponse(
    httpsli::requests::http::HttpRequest &)>;

class HandlerPair {
public:
  HandlerPair(const std::string &address, const Handler &handler);

  bool IsMatching(const std::string &address) const;

  Handler GetHandler() const;

private:
  std::regex address_;
  Handler handler_;
};

class AddressRouter {
public:
  AddressRouter(const std::vector<HandlerPair> &handlers);

  std::optional<Handler>
  FindHandler(httpsli::requests::http::HttpRequest &request) const;

  void AddHandler(const HandlerPair &handler);

private:
  std::vector<HandlerPair> handlers_;
};
} // namespace httpsli::helpers::http