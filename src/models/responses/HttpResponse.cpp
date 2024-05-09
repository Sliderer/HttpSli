#include "HttpResponse.hpp"
#include <sstream>

namespace httpsli::responses::http {

void HttpResponse::AddHeader(std::string &header, std::string &header_value) {
  headers_[header] = header_value;
}

void HttpResponse::SetHeaders(Headers &headers) { headers_ = headers; }

void HttpResponse::SetBody(std::string &body) { body_ = body; }

void HttpResponse::SetStatusCode(int status_code) {
  status_code_ = status_code;
}

void HttpResponse::SetResultMessage(const std::string& result_message){
  result_message_ = result_message;
}

std::string HttpResponse::Serialize() {
  std::stringstream response;

  response << "HTTP/1.1 " << status_code_ << " " << result_message_ << "\r\n";

  for (auto& [header, value] : headers_){
    response << header << ": " << value << "\r\n";
  }

  response << "\r\n";

  response  << body_.str();
  return response.str();
}
} // namespace httpsli::responses::http