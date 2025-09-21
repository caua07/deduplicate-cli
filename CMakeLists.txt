cmake_minimum_required(VERSION 3.10)

project(deduplicate VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(nodupli_lib src/nodupli.cpp src/nodupli.h)

find_package(OpenSSL REQUIRED)
target_link_libraries(nodupli_lib PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_executable(deduplicate src/main.cpp)

target_link_libraries(deduplicate PRIVATE nodupli_lib)


include(GNUInstallDirs)

install(TARGETS deduplicate
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
    DESTINATION .
)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A CLI tool for finding and removing duplicate files.")
set(CPACK_PACKAGE_VENDOR "cauaeyer")
set(CPACK_PACKAGE_CONTACT "cauaeyer")

set(CPACK_GENERATOR "ZIP")

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-macos-arm64")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/dist")
include(CPack)