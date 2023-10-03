#include "http_server.h"
#include <algorithm>
#include <iostream>

const auto LISTEN_BACKLOG = 50;
const auto HOST = "127.0.0.1";
const auto PORT = 8080;

int main() {

  std::cout << "Starting server\n";
  Keeler::HttpServer http(HOST, PORT);

  http.RegisterHandler(Keeler::HttpMethod::GET, "/", [](const Keeler::HttpRequest& req) {
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

    return Keeler::HttpResponse{resp};
  });

  std::cout << "Listening on " << HOST << " " << PORT << "\n";
  http.Serve();

  std::cout << "Shutting down server\n";

  return 0;
}
