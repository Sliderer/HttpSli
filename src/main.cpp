#include <basic_servers/TCPServer.hpp>

int main(){
    httpsli::tcp_server::TCPServer server(std::move("127.0.0.1"), 80);
}