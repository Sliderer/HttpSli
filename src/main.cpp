#include <iostream>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>
#include <built_servers/HttpServer.hpp>

int main(){
    std::string address = "127.0.0.1";
    httpsli::http::HttpServer server(address, 8080);
    return 0;
}