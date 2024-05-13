#include "HttpRequest.hpp"
#include <enums/RequestType.hpp>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <sstream>
#include <vector>

namespace httpsli::requests::http {

void HttpRequest::AddHeader(const std::string &header, const std::string &header_value) {
  headers_[header] = header_value;
}

void HttpRequest::SetHeaders(const Headers &headers) { headers_ = headers; }

void HttpRequest::SetBody(const std::string &body) { body_ = body; }

void HttpRequest::SetRequestType(const RequestType& request_type) {
  request_type_ = request_type;
}

std::string HttpRequest::GetAddress() const { return address_; }

std::string HttpRequest::Serialize() const {
  std::stringstream request;

  std::string request_type_string = EnumToString::Get(request_type_);

  request << request_type_string << " " << address_ << " HTTP/1.1\r\n";

  for (auto& [header, value] : headers_){
    request << header << ": " << value << "\r\n";
  }

  if (body_.has_value()){
    request << "Content-Length: " << body_.value().size() << "\r\n";
  }

  request << "\r\n";

  if (body_.has_value()){
    request << body_.value();
  }

  return request.str();
}

HttpRequest
HttpRequestConstructor::Construct(const std::string &request_string) {

  std::vector<std::string> request_lines;

  boost::split(request_lines, request_string, boost::is_any_of("\n"));

  std::vector<std::string> first_string_parts;

  boost::split(first_string_parts, request_lines[0], boost::is_any_of(" "));

  std::string request_type_string = first_string_parts[0];
  httpsli::http::RequestType request_type =
      httpsli::http::StringToEnum::Get(request_type_string);
  std::string address = first_string_parts[1];
  std::string http_version = first_string_parts[2];

  Headers headers;

  int line_index = 1;
  for (; line_index < request_lines.size(); ++line_index) {
    std::vector<std::string> header_parts;

    boost::split(header_parts, request_lines[line_index],
                 boost::is_any_of(": "));

    if (header_parts.size() != 2) {
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

  return HttpRequest(request_type, address, headers, body);
}
} // namespace httpsli::requests::http