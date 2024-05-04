#include <models/responses/BasicResponse.hpp>
#include <models/requests/BasicRequest.hpp>

#include <memory>
#include <string>
#include <functional>

#include <boost/asio.hpp>


namespace httpsli::tcp_server{
    using namespace httpsli::responses;
    using namespace httpsli::requests;

    using ClientSession = std::function<void (boost::asio::ip::tcp::socket&)>;

    class TCPServer{
        public:
            TCPServer(std::string&& address, int port, ClientSession client_session);

            void StartServer();

            ~TCPServer();
        private:
        class Impl;
            std::unique_ptr<Impl> impl_;
    };
}