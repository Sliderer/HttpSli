#include "HttpRequest.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <iostream>
#include <vector>

namespace httpsli::requests::http {

void HttpRequest::AddHeader(std::string &header, std::string &header_value) {
  headers_[header] = header_value;
}

void HttpRequest::SetHeaders(Headers &headers) { headers_ = headers; }

void HttpRequest::SetBody(std::string &body) { body_ = body; }

void HttpRequest::SetRequestType(RequestType request_type) {
  request_type_ = request_type;
}

std::string HttpRequest::GetAddress() { return address_; }

HttpRequest
HttpRequestConstructor::Construct(const std::string &request_string) {

  std::vector<std::string> request_lines;

  boost::split(request_lines, request_string, boost::is_any_of("\n"));

  std::vector<std::string> first_string_parts;

  boost::split(first_string_parts, request_lines[0], boost::is_any_of(" "));

  std::string request_type_string = first_string_parts[0];
  httpsli::http::RequestType request_type =
      httpsli::http::EnumGetter::GetRequestType(request_type_string);
  std::string address = first_string_parts[1];
  std::string http_version = first_string_parts[2];

  Headers headers;

  int line_index = 1;
  for (; line_index < request_lines.size(); ++line_index) {
    std::vector<std::string> header_parts;

    boost::split(header_parts, request_lines[line_index],
                 boost::is_any_of(": "));

    if (header_parts.size() != 2){
      line_index++;
      break;
    }
    std::string header = header_parts[0], value = header_parts[1];

    headers[header] = value;
  }

   std::string body;

  for (; line_index < request_lines.size(); ++line_index) {
    body += request_lines[line_index];
  }

  return HttpRequest(request_type, headers, address, body);
}
} // namespace httpsli::requests::http