#include <basic_servers/TCPServer.hpp>
#include <models/HttpRequest.hpp>
#include <iostream>

int main(){
    httpsli::tcp_server::TCPServer server(std::move("127.0.0.1"), 8080);
    server.StartServer();
    server.Join();
    std::cout << "End\n";
    return 0;
}