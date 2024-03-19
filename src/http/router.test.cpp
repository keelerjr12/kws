#include "router.h"
#include <gtest/gtest.h>
#include <kws/http_method.h>
#include <kws/http_request.h>
#include <kws/http_response.h>
#include <kws/http_status_code.h>

namespace {

class RouterTest : public testing::TestWithParam<std::string>
{};

constexpr std::array<std::string, 4> route_patterns = {"/",
                                                       "/first",
                                                       "/first/second",
                                                       "/first/{id}"};
INSTANTIATE_TEST_SUITE_P(RoutePatterns,
                         RouterTest,
                         testing::ValuesIn(route_patterns));

TEST_P(RouterTest, EmptyRouter_RouteNonexistent_ReturnsNotFound)
{
    KWS::Http::Router router;
    KWS::Http::HttpRequest request{KWS::Http::HttpMethod::GET, GetParam()};

    const auto resp = router.Execute(request);

    ASSERT_EQ(resp.Status(), KWS::Http::HttpStatusCode::NOT_FOUND);
}

TEST_P(RouterTest, SingleRouteDefinition_RouteNonexistent_ReturnsNotFound)
{
    KWS::Http::Router router;
    KWS::Http::HttpRequest request{KWS::Http::HttpMethod::GET, GetParam()};

    router.RegisterRoute(
        {KWS::Http::HttpMethod::GET, "/dummy"},
        []([[maybe_unused]] const KWS::Http::HttpRequest& req) {
            return KWS::Http::HttpResponse{KWS::Http::HttpStatusCode::OK, ""};
        });
    const auto resp = router.Execute(request);

    ASSERT_EQ(resp.Status(), KWS::Http::HttpStatusCode::NOT_FOUND);
}

TEST_P(RouterTest, SingleRouteDefinition_RouteExists_DoesNotThrowException)
{
    KWS::Http::Router router;
    KWS::Http::HttpRequest request{KWS::Http::HttpMethod::GET, GetParam()};

    std::cout << GetParam() << std::endl;
    router.RegisterRoute(
        {KWS::Http::HttpMethod::GET, GetParam()},
        []([[maybe_unused]] const KWS::Http::HttpRequest& req) {
            return KWS::Http::HttpResponse{KWS::Http::HttpStatusCode::OK, ""};
        });

    ASSERT_NO_THROW(router.Execute(request));
}

TEST_P(RouterTest, SingleRouteDefinition_RouteExists_FunctionCalled)
{
    KWS::Http::Router router;
    KWS::Http::HttpRequest request{KWS::Http::HttpMethod::GET, GetParam()};
    bool isCalled = false;
    ;
    router.RegisterRoute(
        {KWS::Http::HttpMethod::GET, GetParam()},
        [&]([[maybe_unused]] const KWS::Http::HttpRequest& req) {
            isCalled = true;
            return KWS::Http::HttpResponse{KWS::Http::HttpStatusCode::OK, ""};
        });

    router.Execute(request);

    ASSERT_TRUE(isCalled);
}

TEST_P(RouterTest, SingleRouteDefinition_RouteExists_200StatusReturned)
{
    KWS::Http::Router router;
    KWS::Http::HttpRequest request{KWS::Http::HttpMethod::GET, GetParam()};
    router.RegisterRoute(
        {KWS::Http::HttpMethod::GET, GetParam()},
        []([[maybe_unused]] const KWS::Http::HttpRequest& req) {
            return KWS::Http::HttpResponse{KWS::Http::HttpStatusCode::OK, ""};
        });

    const auto resp = router.Execute(request);

    ASSERT_EQ(resp.Status(), KWS::Http::HttpStatusCode::OK);
}

TEST_P(RouterTest, MultipleRouteDefinitions_RouteExists_DoesNotThrowException)
{
    KWS::Http::Router router;
    KWS::Http::HttpRequest request{KWS::Http::HttpMethod::GET, GetParam()};

    for (const auto& route : route_patterns)
    {
        router.RegisterRoute(
            {KWS::Http::HttpMethod::GET, route},
            []([[maybe_unused]] const KWS::Http::HttpRequest& req) {
                return KWS::Http::HttpResponse{KWS::Http::HttpStatusCode::OK,
                                               ""};
            });
    }

    ASSERT_NO_THROW(router.Execute(request));
}

TEST_P(RouterTest, MultipleRouteDefinition_RouteExists_FunctionCalled)
{
    KWS::Http::Router router;
    KWS::Http::HttpRequest request{KWS::Http::HttpMethod::GET, GetParam()};
    bool isCalled = false;
    ;

    for (const auto& route : route_patterns)
    {
        router.RegisterRoute(
            {KWS::Http::HttpMethod::GET, route},
            [&]([[maybe_unused]] const KWS::Http::HttpRequest& req) {
                isCalled = true;
                return KWS::Http::HttpResponse{KWS::Http::HttpStatusCode::OK,
                                               ""};
            });
    }

    router.Execute(request);

    ASSERT_TRUE(isCalled);
}

}  // namespace
