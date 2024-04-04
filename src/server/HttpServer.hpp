#pragma once
#include <memory>

namespace server_logic
{
    class Server{
        public:
            Server();
            ~Server();

            void StartServer();

        private:
            class ServerImpl;
            std::unique_ptr<ServerImpl> impl;
    }; 
} // namespace server_logic
