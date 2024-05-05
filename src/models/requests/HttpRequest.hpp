#pragma once

#include <enums/RequestType.hpp>
#include <models/requests/BasicRequest.hpp>
#include <optional>
#include <string>
#include <unordered_map>

namespace httpsli::requests::http {

using namespace httpsli::http;

using Headers = std::unordered_map<std::string, std::string>;

class HttpRequest : public requests::BasicRequest {
public:
  HttpRequest(){}

  HttpRequest(RequestType request_type, Headers headers,
              std::optional<std::string> body = std::nullopt)
      : request_type_(request_type), headers_(headers), body_(body) {}

  void AddHeader(std::string& header_name, std::string& header_value);

  void SetHeaders(Headers& headers);

  void SetBody(std::string& body);
  
  void SetRequestType(RequestType request_type);

  std::string Serialize() override;

private:
  RequestType request_type_;
  Headers headers_;
  std::optional<std::string> body_;
};
} // namespace httpsli::http