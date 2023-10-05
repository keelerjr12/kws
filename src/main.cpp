#include "http.h"
#include "http_server.h"
#include <functional>
#include <iostream>

const auto LISTEN_BACKLOG = 50;
const auto HOST = "127.0.0.1";
const auto PORT = 8080;

KWS::HttpResponse IndexHandler(const KWS::HttpRequest& req);
KWS::HttpResponse ErrorHandler(const KWS::HttpRequest& req);

int main() {

  std::cout << "Starting server\n";
  KWS::HttpServer http(HOST, PORT);

  http.RegisterRoute({KWS::HttpMethod::GET, "/"}, IndexHandler); 
  http.RegisterRoute({KWS::HttpMethod::GET, "/api"}, IndexHandler);

  http.RegisterErrorHandler(KWS::HttpStatusCode::BAD_REQUEST, ErrorHandler);

  std::cout << "Listening on " << HOST << " " << PORT << "\n";
  http.Serve();

  std::cout << "Shutting down server\n";

  return 0;
}

KWS::HttpResponse IndexHandler(const KWS::HttpRequest& req) {
  std::cout << "---- REQUEST START  ----\n";
  std::cout << ToString(req.Method()) << " " << req.URI() << " " << req.Version() <<  std::endl;
  std::cout << req.Header("Host") << std::endl;
  std::cout << "---- REQUEST END  ----\n";

  std::string resp = "  \
  <html>                \
    <body>              \
      <p>Test Page</p>  \
    </body>             \
  </html>";

  return {resp};
}

KWS::HttpResponse ErrorHandler(const KWS::HttpRequest& req) {
  return {"404: Bad request!"};
}
