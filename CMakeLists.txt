set(LIB_NAME "kws")
set(SOURCE_DIR "src")
set(TEST_DIR "tests")
set(DATA_DIR "data")

cmake_minimum_required(VERSION 3.26)
project(${LIB_NAME})

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

add_library(${LIB_NAME} STATIC "")

add_subdirectory(${SOURCE_DIR})

#target_link_libraries(${LIB_NAME} PUBLIC gmp gmpxx mpfr)

#target_link_libraries(
#  ${LIB_NAME}_tests
#  PUBLIC GTest::gtest_main gmp gmpxx mpfr
#)
#
#include(GoogleTest)
#gtest_discover_tests(${LIB_NAME}_tests)

target_compile_options(${LIB_NAME} INTERFACE
  "$<${GCC_LIKE_CXX}:-g;-Wall;-Werror;-pedantic;-Wextra;-Wshadow;-Wsign-conversion;-Wunused>"
  "$<${MSVC_LIKE_CXX}:-W3>"
)

include(GNUInstallDirs)

include_directories(inc/)

install(TARGETS ${LIB_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      )

install(DIRECTORY inc/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}
        FILES_MATCHING
        PATTERN "*.h"
      )


#file(COPY ${DATA_DIR} DESTINATION ${CMAKE_BINARY_DIR})
