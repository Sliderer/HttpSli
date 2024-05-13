#include <boost/algorithm/string.hpp>

#include "HttpResponse.hpp"
#include <boost/algorithm/string/classification.hpp>
#include <cstdlib>
#include <optional>
#include <sstream>
#include <vector>

namespace httpsli::responses::http {

void HttpResponse::AddHeader(const std::string &header,
                             const std::string &header_value) {
  headers_[header] = header_value;
}

void HttpResponse::SetHeaders(const Headers &headers) { headers_ = headers; }

void HttpResponse::SetBody(const std::string &body) { body_ = body; }

void HttpResponse::SetStatusCode(int status_code) {
  status_code_ = status_code;
}

void HttpResponse::SetResultMessage(const std::string &result_message) {
  result_message_ = result_message;
}

std::string HttpResponse::Serialize() const {
  std::stringstream response;

  response << "HTTP/1.1 " << status_code_ << " " << result_message_ << "\r\n";

  for (auto &[header, value] : headers_) {
    response << header << ": " << value << "\r\n";
  }

  if (body_.has_value()) {
    response << "Content-Length: " << body_->size() << "\r\n";
  }

  response << "\r\n";

  if (body_.has_value()) {
    response << body_.value();
  }
  return response.str();
}

HttpResponse
HttpResponseConstructor::Construct(const std::string &response_string) {
  std::vector<std::string> response_lines;
  boost::split(response_lines, response_string, boost::is_any_of("\r\n"));
  std::string first_line = response_lines[0];

  std::vector<std::string> first_line_parts;
  boost::split(first_line_parts, first_line, boost::is_any_of(" "));

  std::string status_code_string = first_line_parts[1];
  std::string response_message = first_line_parts[2];
  
  Headers headers;
  std::optional<std::string> body = std::nullopt;

  int line_index = 1;

  for(;line_index < response_lines.size(); ++line_index){
    std::vector<std::string> parts;
    boost::split(parts, response_lines[line_index], boost::is_any_of(": "));
    if (parts.size() != 2){
      line_index++;
      break;
    }

    std::string header = parts[0];
    std::string value = parts[1];

    headers[header] = value;
  }

  for (; line_index < response_lines.size(); ++line_index){
    body->append(response_lines[line_index]);
  }

  return HttpResponse(atoi(status_code_string.c_str()), response_message, headers);
}
} // namespace httpsli::responses::http