#include "HttpRequest.hpp"

namespace httpsli::requests::http {

    void HttpRequest::AddHeader(std::string& header, std::string& header_value){
        headers_[header] = header_value;
    }

    void HttpRequest::SetHeaders(Headers& headers){
        headers_ = headers;
    }

    void HttpRequest::SetBody(std::string& body){
        body_ = body;
    }

    void HttpRequest::SetRequestType(RequestType request_type){
        request_type_ = request_type;
    }

    std::string HttpRequest::Serialize() {
        return "";
    }
}