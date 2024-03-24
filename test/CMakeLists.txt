cmake_minimum_required(VERSION 3.14)

project(kwsTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(kws REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

#add_executable(kws_test source/kws_test.cpp)
#target_link_libraries(kws_test PRIVATE kws::kws)
#target_compile_features(kws_test PRIVATE cxx_std_20)

#add_test(NAME kws_test COMMAND kws_test)

# ---- End-of-file commands ----

add_folders(Test)
