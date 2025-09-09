cmake_minimum_required(VERSION 3.10)

project(deduplicate VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(OpenSSL REQUIRED)

# ----------------------------------------------------------------------------
# Core Library
# ----------------------------------------------------------------------------

# Create the nodupli_lib library from its source files.
# We include nodupli.h here to make sure it appears in IDEs.
add_library(nodupli_lib nodupli.cpp nodupli.h)

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
add_executable(deduplicate-cmd main.cpp)

# Link the executable to the core library.
target_link_libraries(deduplicate-cmd PRIVATE nodupli_lib)

# ----------------------------------------------------------------------------
# Installation
# ----------------------------------------------------------------------------

# Include GNUInstallDirs to use standard installation directory variables.
# (e.g., CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR)
include(GNUInstallDirs)

# Install the main executable to the standard binary directory (e.g., /usr/local/bin).
install(TARGETS deduplicate-cmd
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install the library to the standard library directory (e.g., /usr/local/lib).
install(TARGETS nodupli_lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install the public header file to the standard include directory (e.g., /usr/local/include).
install(FILES nodupli.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
