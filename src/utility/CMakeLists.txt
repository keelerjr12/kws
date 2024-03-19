target_sources(${LIB_NAME} PUBLIC
  str_util.cpp
)

file(GLOB TEST_SOURCES *.test.cpp)
target_sources(${TEST_PROJ} PRIVATE ${TEST_SOURCES})
