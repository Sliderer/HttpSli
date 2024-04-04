#include <iostream>
#include <server/sockets/SocketWrapper.hpp>
#include <server/HttpServer.hpp>

int main(){
    server_logic::Server server;
    server.StartServer();
    std::cout << "Hello world";
}