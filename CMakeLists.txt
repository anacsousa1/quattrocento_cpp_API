cmake_minimum_required(VERSION 3.20.3)
project(QuattrocentoAPI VERSION 0.1)

set(CMAKE_DEBUG_POSTFIX "d")
include(GNUInstallDirs)

add_subdirectory(Sockets)
add_subdirectory(tests)
add_subdirectory(OTComm)


#add_subdirectory(OTCommQuattro)

set_target_properties(OTComm PROPERTIES COMMON_LANGUAGE_RUNTIME "")

set_property(TARGET OTComm PROPERTY VS_DOTNET_REFERENCES
    "Microsoft.CSharp"
    "System"
    "System.Core"
    "System.Data"
    "System.Deployment"
    "System.Drawing"
    "System.Xml"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

