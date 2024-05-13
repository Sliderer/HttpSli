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
  HttpResponse() : status_code_(200), result_message_("OK") {}

  HttpResponse(int status_code, const std::string &result_message,
               Headers headers, std::optional<std::string> body = std::nullopt)
      : status_code_(status_code), result_message_(result_message),
        headers_(headers), body_(body) {}

  void AddHeader(const std::string &header_name, const std::string &header_value);

  void SetHeaders(Headers &headers);

  void SetBody(const std::string &body);

  void SetStatusCode(int status_code);

  void SetResultMessage(const std::string &result_message);

  std::string Serialize() const override;

private:
  int status_code_;
  std::string result_message_;
  Headers headers_;
  std::optional<std::string> body_;
};

class HttpResponseConstructor {
  public:
    static HttpResponse Construct(const std::string& response_string);
};
} // namespace httpsli::responses::http