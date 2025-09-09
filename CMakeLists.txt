cmake_minimum_required(VERSION 3.10)

project(deduplicate VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Static Linking Configuration ---
# This tells CMake's FindOpenSSL module to look for and use the static libraries
# (.a) instead of the dynamic ones (.so, .dylib). This is crucial for creating
# a portable executable with no external OpenSSL dependency.
set(OPENSSL_USE_STATIC_LIBS TRUE)

# Find required packages
find_package(OpenSSL REQUIRED)

# ----------------------------------------------------------------------------
# Core Library
# ----------------------------------------------------------------------------

# Create the nodupli_lib library from its source files.
# We include nodupli.h here to make sure it appears in IDEs.
add_library(nodupli_lib src/nodupli.cpp src/nodupli.h)

# Link the library to its dependencies.
target_link_libraries(nodupli_lib PRIVATE OpenSSL::Crypto)

# Make the current directory a public include directory for the library.
# This means that any target that links to nodupli_lib will automatically
# have this directory in its include path.
target_include_directories(nodupli_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------
# Main Executable
# ----------------------------------------------------------------------------

# Create the main executable.
add_executable(deduplicate src/main.cpp)

# Link the executable to the core library.
target_link_libraries(deduplicate PRIVATE nodupli_lib)

# ----------------------------------------------------------------------------
# Installation
# ----------------------------------------------------------------------------

# Include GNUInstallDirs to use standard installation directory variables.
# (e.g., CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR)
include(GNUInstallDirs)

# Install the main executable to the standard binary directory (e.g., /usr/local/bin).
install(TARGETS deduplicate
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# We don't need to install the library or headers for a binary release.
# You can uncomment these if you want to provide a developer package.
# install(TARGETS nodupli_lib
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )
# install(FILES nodupli.h
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# ----------------------------------------------------------------------------
# Packaging with CPack
# ----------------------------------------------------------------------------

# Install the README and LICENSE files to the root of the package.
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

# Use the ZIP generator for creating the archive.
# Other options include TGZ, etc.
set(CPACK_GENERATOR "ZIP")

# This ensures the package has a top-level directory, e.g., "deduplicate-1.0-Darwin".
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-macos-arm64")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/dist")
include(CPack)
