cmake_minimum_required(VERSION 3.20.3)
project(QuattrocentoAPI VERSION 0.1)

set(CMAKE_DEBUG_POSTFIX "d")
include(GNUInstallDirs)

add_subdirectory(Sockets)
add_subdirectory(tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# add_executable(${PROJECT_NAME} main.cpp)


# # Create ConcurrencyConfig.cmake
# set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/Filter)

# include(CMakePackageConfigHelpers)
# configure_package_config_file(FilterConfig.cmake.in
#                               ${CMAKE_CURRENT_BINARY_DIR}/FilterConfigForInstall.cmake
#                               INSTALL_DESTINATION ${ConfigPackageLocation}
#                               PATH_VARS CMAKE_INSTALL_INCLUDEDIR)

# write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/FilterConfigVersion.cmake
#                                     VERSION 0.1
#                                     COMPATIBILITY ExactVersion )

# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/FilterConfigForInstall.cmake
#               DESTINATION ${ConfigPackageLocation}
#               RENAME FilterConfig.cmake)

# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/FilterConfigVersion.cmake
#               DESTINATION ${ConfigPackageLocation})

# install(EXPORT Filter_Exports NAMESPACE rtb::
#                               DESTINATION ${ConfigPackageLocation})

# # Uninstall target
# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#     IMMEDIATE @ONLY)

# add_custom_target(uninstall
#     COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
