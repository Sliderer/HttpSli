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

std::string HttpResponse::Serialize() {
  std::stringstream response;
  std::stringstream response_body; 
  response_body << "<title>Test C++ HTTP Server</title>\n"
                << "<h1>Test page</h1>\n"
                << "<p>This is body of the test page...</p>\n"
                << "<h2>Request headers</h2>\n"
                << "<pre>" << "dd" << "</pre>\n"
                << "<em><small>Test C++ Http Server</small></em>\n";

  response << "HTTP/1.1 200 OK\r\n"
           << "Version: HTTP/1.1\r\n"
           << "Content-Type: text/plain; charset=utf-8\r\n"
           << "Content-Length: " << response_body.str().length() << "\r\n\r\n"
           << response_body.str();
  return response.str();
}
} // namespace httpsli::responses::http