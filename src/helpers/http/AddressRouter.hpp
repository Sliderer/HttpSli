#pragma once

#include <vector>
#include <functional>
#include <regex>
#include <optional>

#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>

namespace httpsli::helpers::http {

    using Handler = std::function<httpsli::responses::http::HttpResponse(httpsli::requests::http::HttpRequest&)>;

    class HandlerPair {
        public: 
        bool IsMatching(const std::string& address) const;

        Handler GetHandler() const;

        private:
            std::regex address_;
            Handler handler_; 
    };

    class AddressRouter{
        public:
        
        std::optional<Handler> FindHandler(httpsli::requests::http::HttpRequest& request) const;

        private:
            std::vector<HandlerPair> handlers_;
    };
}