if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/kws-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package kws)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT kws_Development
)

install(
    TARGETS kws_kws
    EXPORT kwsTargets
    RUNTIME #
    COMPONENT kws_Runtime
    LIBRARY #
    COMPONENT kws_Runtime
    NAMELINK_COMPONENT kws_Development
    ARCHIVE #
    COMPONENT kws_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    kws_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE kws_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(kws_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${kws_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT kws_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${kws_INSTALL_CMAKEDIR}"
    COMPONENT kws_Development
)

install(
    EXPORT kwsTargets
    NAMESPACE kws::
    DESTINATION "${kws_INSTALL_CMAKEDIR}"
    COMPONENT kws_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
