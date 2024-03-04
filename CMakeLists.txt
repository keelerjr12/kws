set(LIB_NAME "kws")
set(SOURCE_DIR "src")
set(INCLUDE_DIR "inc")
set(EXAMPLES_DIR "examples")
set(DATA_DIR "data")

cmake_minimum_required(VERSION 3.26)
project(${LIB_NAME})

message("Build type is: ${CMAKE_BUILD_TYPE}")

option(BUILD_TESTS "Enable test building; users may want to turn this OFF.)" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GCC_LIKE_CXX "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(MSVC_LIKE_CXX "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,-cppcoreguidelines-pro-type-reinterpret-cast,hicpp-*,modernize-*,-modernize-use-trailing-return-type,performance-*,readability-*)

add_library(${LIB_NAME} SHARED "")

target_compile_options(${LIB_NAME} PUBLIC
  "$<${GCC_LIKE_CXX}:-g;-Wall;-Werror;-pedantic;-Wextra;-Wshadow;-Wsign-conversion;-Wunused>"
  "$<${MSVC_LIKE_CXX}:-W3>"
)

file(GLOB PUBLIC_HEADERS ${INCLUDE_DIR}/**/*.h)
set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

target_include_directories(${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${INCLUDE_DIR}/${LIB_NAME}>
)

include(GNUInstallDirs)
install(TARGETS ${LIB_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}
)

if (BUILD_TESTS)
  message(STATUS "Testing is on")

  enable_testing()
  set(TEST_PROJ "tests")

  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )

  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  add_executable(${TEST_PROJ} "")

  target_link_libraries(
    ${TEST_PROJ}
    kws
    GTest::gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(${TEST_PROJ})
endif()

add_subdirectory(${INCLUDE_DIR})
add_subdirectory(${SOURCE_DIR})
add_subdirectory(${EXAMPLES_DIR})
