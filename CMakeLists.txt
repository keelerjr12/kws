set(PROJECT_NAME "kws")
set(SOURCE_DIR "src")
set(TEST_DIR "tests")
set(DATA_DIR "data")

cmake_minimum_required(VERSION 3.26)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GCC_LIKE_CXX "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(MSVC_LIKE_CXX "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,-cppcoreguidelines-pro-type-reinterpret-cast,hicpp-*,modernize-*,-modernize-use-trailing-return-type,performance-*,readability-*)

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} 
  ${SOURCE_DIR}/main.cpp 
  ${SOURCE_DIR}/tcp_server.cpp 
  ${SOURCE_DIR}/tcp_stream.cpp 
  ${SOURCE_DIR}/http_server.cpp 
  ${SOURCE_DIR}/http.cpp 
)

#target_link_libraries(${PROJECT_NAME} PUBLIC gmp gmpxx mpfr)

#add_executable(
#  ${PROJECT_NAME}_tests
#  ${TEST_DIR}/numeric/precise_number_test.cpp
#  ${SOURCE_DIR}/numeric/precise_number.cpp 
#  ${TEST_DIR}/kinematics/velocity_test.cpp
#  ${SOURCE_DIR}/kinematics/velocity.cpp 
#)
#
#target_link_libraries(
#  ${PROJECT_NAME}_tests
#  PUBLIC GTest::gtest_main gmp gmpxx mpfr
#)
#
#include(GoogleTest)
#gtest_discover_tests(${PROJECT_NAME}_tests)

target_compile_options(${PROJECT_NAME} INTERFACE
  "$<${GCC_LIKE_CXX}:-g;-Wall;-Werror;-pedantic;-Wextra;-Wshadow;-Wsign-conversion;-Wunused>"
  "$<${MSVC_LIKE_CXX}:-W3>"
)

#file(COPY ${DATA_DIR} DESTINATION ${CMAKE_BINARY_DIR})
