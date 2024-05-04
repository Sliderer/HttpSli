#pragma once

#include <basic_servers/TCPServer.hpp>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>

#include <functional>

namespace httpsli::http {

    using ClientSession = std::function<responses::http::HttpResponse(requests::http::HttpRequest&)>;


    class HttpServer : public TCPServer {
        public:
            HttpServer();

        private:
            requests::http::HttpRequest ParseRequest();

            responses::http::HttpResponse BuildResponse();


        private:
            ClientSession client_session_;

    };
}