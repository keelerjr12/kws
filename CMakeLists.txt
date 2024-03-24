cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    kws
    VERSION 0.1.0
    DESCRIPTION "Multithreaded http server"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    kws_kws
    ""
)
add_library(kws::kws ALIAS kws_kws)

include(GenerateExportHeader)
generate_export_header(
    kws_kws
    BASE_NAME kws
    EXPORT_FILE_NAME export/kws/kws_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(kws_kws PUBLIC KWS_STATIC_DEFINE)
endif()

set_target_properties(
    kws_kws PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME kws
    OUTPUT_NAME kws
)

add_subdirectory(source)

target_include_directories(
    kws_kws ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    kws_kws SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(kws_kws PUBLIC cxx_std_20)

# ---- Build Tests ----


# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${kws_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT kws_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of kws"
  )
endif()

include(cmake/dev-mode.cmake)
