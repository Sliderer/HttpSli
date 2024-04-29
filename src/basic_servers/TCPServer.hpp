#include <memory>
#include <string>

namespace httpsli::tcp_server{
    class TCPServer{
        public:
            TCPServer(std::string&& address, int port);

            int StartServer() {
                
                return 0;
            }

            ~TCPServer();

        private:
        class Impl;
            std::unique_ptr<Impl> impl_;
    };
}