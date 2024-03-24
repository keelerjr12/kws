#include <exception>
#include <iostream>
#include <string>

#include <kws/http_method.h>
#include <kws/http_request.h>
#include <kws/http_response.h>
#include <kws/http_route.h>
#include <kws/http_server.h>
#include <kws/http_status_code.h>

const auto host = "127.0.0.1";
const auto port = 8080;

KWS::Http::HttpResponse IndexHandler(const KWS::Http::HttpRequest& req);
KWS::Http::HttpResponse UserHandler(const KWS::Http::HttpRequest& req);

KWS::Http::HttpResponse ErrorHandler(const KWS::Http::HttpRequest& req);

int main()
{
  try {
    std::cout << "Starting server\n";
    KWS::Http::HttpServer http(host, port);

    http.RegisterRoute({KWS::Http::HttpMethod::GET, "/"}, IndexHandler);
    http.RegisterRoute({KWS::Http::HttpMethod::GET, "/users"}, IndexHandler);
    http.RegisterRoute({KWS::Http::HttpMethod::GET, "/users/{id}"},
                       UserHandler);
    http.RegisterRoute({KWS::Http::HttpMethod::GET, "/users/{id}/profile"},
                       IndexHandler);

    http.RegisterErrorHandler(KWS::Http::HttpStatusCode::NotFound,
                              ErrorHandler);

    std::cout << "Listening on " << host << " " << port << '\n';
    http.Serve();

    std::cout << "Shutting down server\n";
  } catch (const std::exception& exp) {
    std::cout << exp.what() << '\n';
  }

  return 0;
}

KWS::Http::HttpResponse IndexHandler(const KWS::Http::HttpRequest& req)
{
  std::cout << "---- REQUEST START  ----\n";
  std::cout << ToString(req.Method()) << " " << req.Path() << " "
            << req.Version() << '\n';
  std::cout << req.Header("Host") << '\n';
  std::cout << "---- REQUEST END  ----\n";

  const std::string resp =
      "  \
  <html>                \
    <body>              \
      <p>Welcome to the homepage</p>  \
    </body>             \
  </html>";

  return {KWS::Http::HttpStatusCode::OK, resp};
}

KWS::Http::HttpResponse UserHandler(const KWS::Http::HttpRequest& req)
{
  const auto userId = req.RouteValue<int>("id");

  const std::string resp =
      "  \
  <html>                \
    <body>              \
      <p>Welcome user " + std::to_string(userId) + "</p>  \
    </body>             \
  </html>";

  return {KWS::Http::HttpStatusCode::OK, resp};
}

KWS::Http::HttpResponse ErrorHandler(
    [[maybe_unused]] const KWS::Http::HttpRequest& req)
{
  return {KWS::Http::HttpStatusCode::BadRequest, "404: Bad request!"};
}
