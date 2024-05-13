
#include "basic_clients/TCPClient.hpp"
#include "built_clients/HttpClient.hpp"
#include "enums/RequestType.hpp"
#include <built_servers/HttpServer.hpp>
#include <helpers/http/AddressRouter.hpp>
#include <iostream>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>
#include <thread>

int main() {
  // httpsli::helpers::http::HandlerPair handler1(
  //     "/", [](httpsli::requests::http::HttpRequest &request) {
  //       httpsli::responses::http::HttpResponse r;
  //       std::string body = "home";
  //       r.SetBody(body);
  //       return r;
  //     });

  // httpsli::helpers::http::HandlerPair handler2(
  //     "/local[0-9]*", [](httpsli::requests::http::HttpRequest &request) {
  //       httpsli::responses::http::HttpResponse r;
  //       std::string body = "local";
  //       r.SetBody(body);
  //       return r;
  //     });

  // httpsli::helpers::http::AddressRouter router({handler1, handler2});
  // std::string address = "127.0.0.1";
  // httpsli::http::HttpServer server(address, 8081, router);
  // server.StartServer();
  // server.Join();

  // httpsli::tcp_client::TCPClient client("127.0.0.1", 80);
  httpsli::requests::http::HttpRequest r(httpsli::http::RequestType::GET, "/", {{"Host", "reader.com"}});
  // client.SendRequest(r);
  // client.Join();

  httpsli::http::HttpClient client("127.0.0.1", 80);
  client.SendRequest(r, [](const boost::system::error_code &error, std::size_t bytes_transferred,
    const std::shared_ptr<char[]> & buffer){
      std::cout << buffer.get() << '\n';
    });
  client.Join();

  return 0;
}