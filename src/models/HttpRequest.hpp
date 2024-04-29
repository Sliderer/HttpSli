#pragma once

#include <unordered_map>
#include <string>
#include <enums/RequestType.hpp>

namespace httpsli::http{
    class HttpRequest{
        private:
            std::unordered_map<std::string, std::string> headers_;
            RequestType request_type_;
    };
}