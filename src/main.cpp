#include <iostream>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>
#include <built_servers/HttpServer.hpp>
#include <helpers/http/AddressRouter.hpp>

int main(){
    httpsli::helpers::http::AddressRouter router;
    std::string address = "127.0.0.1";
    httpsli::http::HttpServer server(address, 8080, router);
    return 0;
}