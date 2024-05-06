#include "HttpServer.hpp"

#include <optional>
#include <iostream>

namespace httpsli::http {
    HttpServer::HttpServer(std::string& address, int port, httpsli::helpers::http::AddressRouter router) : 
        httpsli::tcp_server::TCPServer(address, port, [&](boost::asio::ip::tcp::socket& socket){
                this->ClientSession(socket);
            }), router_(router) {}

    void HttpServer::ClientSession(boost::asio::ip::tcp::socket& socket) {
        requests::http::HttpRequest request = GetRequest(socket);

        std::optional<Handler> handler = router_.FindHandler(request);
        if (!handler.has_value()){
            std::cout << "No handler found\n";
            return;
        }
        responses::http::HttpResponse response = (*handler)(request);

        SendResponse(response);
    }
    
    requests::http::HttpRequest HttpServer::GetRequest(boost::asio::ip::tcp::socket& socket){
        return requests::http::HttpRequest();
    }

    int HttpServer::SendResponse(responses::http::HttpResponse& response){

        return 0;
    }
}