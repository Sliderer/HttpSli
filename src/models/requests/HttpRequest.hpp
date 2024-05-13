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
  HttpRequest() {}

  HttpRequest(const RequestType &request_type, const std::string &address,
              const Headers &headers,
              const std::optional<std::string> &body = std::nullopt)
      : request_type_(request_type), address_(address), headers_(headers),
        body_(body) {}

  void AddHeader(const std::string &header_name,
                 const std::string &header_value);

  void SetHeaders(const Headers &headers);

  void SetBody(const std::string &body);

  void SetRequestType(const RequestType &request_type);

  std::string GetAddress() const;

  std::string Serialize() const override;

private:
  RequestType request_type_;
  Headers headers_;
  std::string address_;
  std::optional<std::string> body_;
};

class HttpRequestConstructor {
public:
  static HttpRequest Construct(const std::string &request_string);
};

} // namespace httpsli::requests::http