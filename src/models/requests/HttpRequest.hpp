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
              const std::string& address, 
              std::optional<std::string> body = std::nullopt)
      : request_type_(request_type), address_(address), headers_(headers), body_(body) {}

  void AddHeader(std::string& header_name, std::string& header_value);

  void SetHeaders(Headers& headers);

  void SetBody(std::string& body);
  
  void SetRequestType(RequestType request_type);

  std::string GetAddress();

  std::string Serialize() override;

private:
  RequestType request_type_;
  Headers headers_;
  std::string address_;
  std::optional<std::string> body_;
};


class HttpRequestConstructor {
  public:
    static HttpRequest Construct(const std::string& request_string);
};


} // namespace httpsli::http