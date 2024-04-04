#include "HttpServer.hpp"
#include <iostream>

namespace server_logic {
    class Server::ServerImpl{
        public:
            ServerImpl(){}

            void StartServer(){
                std::cout << "Start server";
            }

            ~ServerImpl(){}
    };

    void Server::StartServer(){
        impl->StartServer();
    }

    Server::Server() : impl(new ServerImpl()){}

    Server::~Server() {}
}