#include "router.h"
#include <gtest/gtest.h>
#include <kws/http_method.h>
#include <kws/http_request.h>
#include <kws/http_response.h>
#include <kws/http_status_code.h>
#include <stdexcept>

namespace {

class RouterTest : public testing::TestWithParam<std::string>
{};

constexpr std::array<std::string, 3> route_patterns = {"/",
                                                       "/first",
                                                       "/first/second"};
INSTANTIATE_TEST_SUITE_P(RoutePatterns,
                         RouterTest,
                         testing::ValuesIn(route_patterns));

TEST_P(RouterTest, EmptyRouterRouteNonexistentThrowsException)
{
    KWS::Router router;
    KWS::HttpRequest request{KWS::HttpMethod::GET, RouterTest::GetParam()};

    ASSERT_THROW(router.Execute(request), std::runtime_error);
}

TEST_P(RouterTest, SingleRouteDefinitionRouteNonexistentThrowsException)
{
    KWS::Router router;
    KWS::HttpRequest request{KWS::HttpMethod::GET, RouterTest::GetParam()};

    router.RegisterRoute(
        {KWS::HttpMethod::GET, "/dummy"},
        []([[maybe_unused]] const KWS::HttpRequest& req) {
            return KWS::HttpResponse{KWS::HttpStatusCode::OK, ""};
        });

    ASSERT_THROW(router.Execute(request), std::runtime_error);
}

TEST_P(RouterTest, SingleRouteDefinitionRouteExistsDoesNotThrowException)
{
    KWS::Router router;
    KWS::HttpRequest request{KWS::HttpMethod::GET, RouterTest::GetParam()};

    router.RegisterRoute(
        {KWS::HttpMethod::GET, RouterTest::GetParam()},
        []([[maybe_unused]] const KWS::HttpRequest& req) {
            return KWS::HttpResponse{KWS::HttpStatusCode::OK, ""};
        });

    ASSERT_NO_THROW(router.Execute(request));
}

TEST_P(RouterTest, MultipleRouteDefinitionsRouteExistsDoesNotThrowException)
{
    KWS::Router router;
    KWS::HttpRequest request{KWS::HttpMethod::GET, RouterTest::GetParam()};

    for (const auto& route : route_patterns)
    {
        router.RegisterRoute(
            {KWS::HttpMethod::GET, route},
            []([[maybe_unused]] const KWS::HttpRequest& req) {
                return KWS::HttpResponse{KWS::HttpStatusCode::OK, ""};
            });
    }

    ASSERT_NO_THROW(router.Execute(request));
}

TEST_P(RouterTest, SingleRouteDefinitionRouteExistsFunctionCalled)
{
    KWS::Router router;
    KWS::HttpRequest request{KWS::HttpMethod::GET, RouterTest::GetParam()};
    bool isCalled = false;
    ;
    router.RegisterRoute(
        {KWS::HttpMethod::GET, RouterTest::GetParam()},
        [&]([[maybe_unused]] const KWS::HttpRequest& req) {
            isCalled = true;
            return KWS::HttpResponse{KWS::HttpStatusCode::OK, ""};
        });

    router.Execute(request);

    ASSERT_TRUE(isCalled);
}

TEST_P(RouterTest, MultipleRouteDefinitionRouteExistsFunctionCalled)
{
    KWS::Router router;
    KWS::HttpRequest request{KWS::HttpMethod::GET, RouterTest::GetParam()};
    bool isCalled = false;
    ;

    for (const auto& route : route_patterns)
    {
        router.RegisterRoute(
            {KWS::HttpMethod::GET, route},
            [&]([[maybe_unused]] const KWS::HttpRequest& req) {
                isCalled = true;
                return KWS::HttpResponse{KWS::HttpStatusCode::OK, ""};
            });
    }

    router.Execute(request);

    ASSERT_TRUE(isCalled);
}

TEST_P(RouterTest, SingleRouteDefinitionRouteExists200StatusReturned)
{
    KWS::Router router;
    KWS::HttpRequest request{KWS::HttpMethod::GET, RouterTest::GetParam()};
    router.RegisterRoute(
        {KWS::HttpMethod::GET, RouterTest::GetParam()},
        []([[maybe_unused]] const KWS::HttpRequest& req) {
            return KWS::HttpResponse{KWS::HttpStatusCode::OK, ""};
        });

    const auto resp = router.Execute(request);

    ASSERT_EQ(resp.Status(), KWS::HttpStatusCode::OK);
}

}  // namespace
