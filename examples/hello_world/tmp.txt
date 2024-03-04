cmake_minimum_required(VERSION 3.26)
project(hello_world)

set(CMAKE_BUILD_RPATH "/usr/local/lib64/")

# Locate the shared library
find_library(KWS_LIBRARY
    NAMES kws
    PATHS /usr/local/lib64
)

# Check if the library was found
if (NOT KWS_LIBRARY)
    message(FATAL_ERROR "The 'kws' library was not found.")
endif()

add_executable(hello_world main.cpp)

include_directories(/usr/local/include/kws/)
target_link_libraries(hello_world PRIVATE kws)

