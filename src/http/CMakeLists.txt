target_sources(${LIB_NAME} PUBLIC
  http_server_impl.h
  http_server_impl.cpp
  http_method.cpp
  http_request.cpp
  http_response.cpp
  http_route.cpp
  http_server.cpp
  router.cpp
  route_template.cpp
)

file(GLOB TEST_SOURCES *.test.cpp)
target_sources(${TEST_PROJ} PRIVATE ${TEST_SOURCES})
