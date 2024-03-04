#include <exception>
#include <iostream>
#include <kws/http_request.h>
#include <kws/http_response.h>
#include <kws/http_route.h>
#include <kws/http_server.h>

const auto HOST = "127.0.0.1";
const auto PORT = 8080;

KWS::HttpResponse IndexHandler(const KWS::HttpRequest& req);
KWS::HttpResponse ErrorHandler(const KWS::HttpRequest& req);

int main()
{
    try
    {
        std::cout << "Starting server\n";
        KWS::HttpServer http(HOST, PORT);

        http.RegisterRoute({KWS::HttpMethod::GET, "/"}, IndexHandler);
        http.RegisterRoute({KWS::HttpMethod::GET, "/users/{id}"}, IndexHandler);
        http.RegisterRoute({KWS::HttpMethod::GET, "/users/{id}/profile"},
                           IndexHandler);

        http.RegisterErrorHandler(KWS::HttpStatusCode::BAD_REQUEST,
                                  ErrorHandler);

        std::cout << "Listening on " << HOST << " " << PORT << "\n";
        http.Serve();

        std::cout << "Shutting down server\n";
    }
    catch (const std::exception& exp)
    {
        std::cout << exp.what() << '\n';
    }

    return 0;
}

KWS::HttpResponse IndexHandler(const KWS::HttpRequest& req)
{
    std::cout << "---- REQUEST START  ----\n";
    std::cout << ToString(req.Method()) << " " << req.Path() << " "
              << req.Version() << std::endl;
    std::cout << req.Header("Host") << std::endl;
    std::cout << "---- REQUEST END  ----\n";

    std::string resp = "  \
  <html>                \
    <body>              \
      <p>Test Page</p>  \
    </body>             \
  </html>";

    return {KWS::HttpStatusCode::OK, resp};
}

KWS::HttpResponse ErrorHandler([[maybe_unused]] const KWS::HttpRequest& req)
{
    return {KWS::HttpStatusCode::BAD_REQUEST, "404: Bad request!"};
}
