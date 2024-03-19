#include <exception>
#include <iostream>
#include <kws/http_request.h>
#include <kws/http_response.h>
#include <kws/http_route.h>
#include <kws/http_server.h>

const auto HOST = "127.0.0.1";
const auto PORT = 8080;

KWS::Http::HttpResponse IndexHandler(const KWS::Http::HttpRequest& req);
KWS::Http::HttpResponse ErrorHandler(const KWS::Http::HttpRequest& req);

int main()
{
    try
    {
        std::cout << "Starting server\n";
        KWS::Http::HttpServer http(HOST, PORT);

        http.RegisterRoute({KWS::Http::HttpMethod::GET, "/"}, IndexHandler);
        http.RegisterRoute({KWS::Http::HttpMethod::GET, "/users"},
                           IndexHandler);
        http.RegisterRoute({KWS::Http::HttpMethod::GET, "/users/{id}"},
                           IndexHandler);
        http.RegisterRoute({KWS::Http::HttpMethod::GET, "/users/{id}/profile"},
                           IndexHandler);

        http.RegisterErrorHandler(KWS::Http::HttpStatusCode::NOT_FOUND,
                                  ErrorHandler);

        std::cout << "Listening on " << HOST << " " << PORT << '\n';
        http.Serve();

        std::cout << "Shutting down server\n";
    }
    catch (const std::exception& exp)
    {
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
    std::cout << req.RouteValue<std::string>("id") << '\n';
    std::cout << "---- REQUEST END  ----\n";

    std::string resp = "  \
  <html>                \
    <body>              \
      <p>Test Page</p>  \
    </body>             \
  </html>";

    return {KWS::Http::HttpStatusCode::OK, resp};
}

KWS::Http::HttpResponse
ErrorHandler([[maybe_unused]] const KWS::Http::HttpRequest& req)
{
    return {KWS::Http::HttpStatusCode::BAD_REQUEST, "404: Bad request!"};
}
