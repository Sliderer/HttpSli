#include <iostream>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>
#include <built_servers/HttpServer.hpp>
#include <helpers/http/AddressRouter.hpp>

int main(){
    httpsli::helpers::http::HandlerPair handler("/local", [](httpsli::requests::http::HttpRequest& request){
        std::cout << "Answer is OK\n";
        return httpsli::responses::http::HttpResponse();
    });
    httpsli::helpers::http::AddressRouter router({handler});
    std::string address = "127.0.0.1";
    httpsli::http::HttpServer server(address, 8081, router);
    server.StartServer();
    return 0;
}