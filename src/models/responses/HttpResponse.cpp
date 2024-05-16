#include <boost/algorithm/string.hpp>

#include "HttpResponse.hpp"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <cstdlib>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include <iostream>

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
  boost::split(response_lines, response_string, boost::is_any_of("\n"));
  std::string first_line = response_lines[0];

  std::vector<std::string> first_line_parts;
  boost::split(first_line_parts, first_line, boost::is_any_of(" "));

  std::string status_code_string = first_line_parts[1];
  std::string response_message = first_line_parts[2];
  
  Headers headers;
  std::optional<std::string> body = "";

  int line_index = 1;
  std::cout << '\n';
  for(;line_index < response_lines.size(); ++line_index){
    auto split_index = response_lines[line_index].find_first_of(':'); 
    if (split_index == std::string::npos){
      line_index++;
      break;
    }

    std::string header = response_lines[line_index].substr(0, split_index);
    std::string value = response_lines[line_index].substr(split_index + 2);
    headers[header] = value;
  }

  for (; line_index < response_lines.size(); ++line_index){
    body->append(response_lines[line_index] + "\n");
  }
 
  return HttpResponse(atoi(status_code_string.c_str()), response_message, headers, body);
}
} // namespace httpsli::responses::http