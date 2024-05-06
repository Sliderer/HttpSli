#pragma once

#include <boost/asio.hpp>

#include <basic_servers/TCPServer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>
#include <helpers/http/AddressRouter.hpp>

#include <functional>

namespace httpsli::http {

    using Handler = std::function<httpsli::responses::http::HttpResponse(httpsli::requests::http::HttpRequest&)>;
    using ClientSession = std::function<responses::http::HttpResponse(requests::http::HttpRequest&)>;

    class HttpServer : public httpsli::tcp_server::TCPServer {
        public:
            HttpServer(std::string& address, int port, httpsli::helpers::http::AddressRouter router);

        private:
            void ClientSession(boost::asio::ip::tcp::socket& socket);

            requests::http::HttpRequest GetRequest(boost::asio::ip::tcp::socket& socket);

            int SendResponse(responses::http::HttpResponse& response);

        private:
            httpsli::helpers::http::AddressRouter router_;
    };
}