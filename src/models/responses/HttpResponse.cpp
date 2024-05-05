#include "HttpResponse.hpp"

namespace httpsli::responses::http {

    void HttpResponse::AddHeader(std::string& header, std::string& header_value){
        headers_[header] = header_value;
    }

    void HttpResponse::SetHeaders(Headers& headers){
        headers_ = headers;
    }

    void HttpResponse::SetBody(std::string& body){
        body_ = body;
    }

    void HttpResponse::SetRequestType(RequestType request_type){
        request_type_ = request_type;
    }

    void HttpResponse::SetStatusCode(int status_code){
        status_code_ = status_code;
    }

    std::string HttpResponse::Serialize() {
        return "";
    }
}