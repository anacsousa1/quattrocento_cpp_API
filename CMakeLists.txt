cmake_minimum_required(VERSION 3.20.3)
project(QuattrocentoAPI VERSION 0.1)

set(CMAKE_DEBUG_POSTFIX "d")
include(GNUInstallDirs)

add_subdirectory(Sockets)
add_subdirectory(tests)
add_subdirectory(OTComm)


#add_subdirectory(OTCommQuattro)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_property(TARGET CoreClrWithCMake PROPERTY DOTNET_TARGET_FRAMEWORK "netcoreapp3.1")

TARGET_LINK_LIBRARIES(${PROJECT_NAME})
