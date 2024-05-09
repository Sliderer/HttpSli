#include <built_servers/HttpServer.hpp>
#include <helpers/http/AddressRouter.hpp>
#include <iostream>
#include <models/requests/HttpRequest.hpp>
#include <models/responses/HttpResponse.hpp>

#include <thread>

int main() {
  httpsli::helpers::http::HandlerPair handler1(
      "/", [](httpsli::requests::http::HttpRequest &request) {
        httpsli::responses::http::HttpResponse r;
        std::string body = "home";
        r.SetBody(body);
        return r;
      });

  httpsli::helpers::http::HandlerPair handler2(
      "/local[0-9]*", [](httpsli::requests::http::HttpRequest &request) {
        httpsli::responses::http::HttpResponse r;
        std::string body = "local";
        r.SetBody(body);
        return r;
      });

  httpsli::helpers::http::AddressRouter router({handler1, handler2});
  std::string address = "127.0.0.1";
  httpsli::http::HttpServer server(address, 8081, router);
  server.StartServer();
  server.Join();

  return 0;
}