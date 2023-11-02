target_sources(${LIB_NAME} PRIVATE
  http_server_impl.cpp
  http_method.cpp
  http_request.cpp
  http_response.cpp
  http_route.cpp
  http_server.cpp
  router.cpp
)

file(GLOB TEST_SOURCES *.test.cpp)
target_sources(${TEST_PROJ} PRIVATE ${TEST_SOURCES})
