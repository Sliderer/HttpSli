#include "HttpServer.hpp"

namespace httpsli::http {

    HttpServer::HttpServer(std::string& address, int port) : 
        httpsli::tcp_server::TCPServer(address, port) {
            SetClientSession([&](boost::asio::ip::tcp::socket& socket){
                this->ClientSession(socket);
            });
    }

    void HttpServer::ClientSession(boost::asio::ip::tcp::socket& socket) {
        requests::http::HttpRequest request = GetRequest(socket);

        responses::http::HttpResponse response; // = Mapper Gethandler and launch handler;

        SendResponse(response);
    }
    
    requests::http::HttpRequest HttpServer::GetRequest(boost::asio::ip::tcp::socket& socket){
        return requests::http::HttpRequest();
    }

    int HttpServer::SendResponse(responses::http::HttpResponse& response){

        return 0;
    }
}