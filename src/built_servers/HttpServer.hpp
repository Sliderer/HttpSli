#pragma once

#include <boost/asio.hpp>

#include <basic_servers/TCPServer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>

#include <functional>

namespace httpsli::http {

    using ClientSession = std::function<responses::http::HttpResponse(requests::http::HttpRequest&)>;

    class HttpServer : public httpsli::tcp_server::TCPServer {
        public:
            HttpServer(std::string& address, int port);

        private:
            void ClientSession(boost::asio::ip::tcp::socket& socket);

            requests::http::HttpRequest GetRequest(boost::asio::ip::tcp::socket& socket);

            int SendResponse(responses::http::HttpResponse& response);

    
    };
}