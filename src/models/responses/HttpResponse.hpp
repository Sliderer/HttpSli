#pragma once

#include <enums/RequestType.hpp>
#include <models/responses/BasicResponse.hpp>
#include <optional>
#include <string>
#include <unordered_map>

namespace httpsli::responses::http {
using namespace httpsli::http;

using Headers = std::unordered_map<std::string, std::string>;

class HttpResponse : public BasicResponse {
public:
  HttpResponse(){}

  HttpResponse(int status_code, const std::string& result_message, Headers headers,
              std::optional<std::string> body = std::nullopt)
      : status_code_(status_code), result_message_(result_message), headers_(headers), body_(body) {}

  void AddHeader(std::string& header_name, std::string& header_value);

  void SetHeaders(Headers& headers);

  void SetBody(std::string& body);
  
  void SetStatusCode(int status_code);

  void SetResultMessage(const std::string& result_message);

  std::string Serialize() override;

private:
  int status_code_;
  std::string result_message_;
  Headers headers_;
  std::optional<std::string> body_;
};
} // namespace httpsli::http