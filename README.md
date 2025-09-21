```
      _          _             _ _           _       _ 
     | |        | |           | (_)         | |     | |
   __| | ___  __| |_   _ _ __ | |_  ___ __ _| |_ ___| |
  / _` |/ _ \/ _` | | | | '_ \| | |/ __/ _` | __/ _ \ |
 | (_| |  __/ (_| | |_| | |_) | | | (_| (_| | ||  __/_|
  \__,_|\___|\__,_|\__,_| .__/|_|_|\___\__,_|\__\___(_)
                        | |                            
                        |_|                            
```                
                        
Blazing light, intuitive and complete cli tool for managing duplicates in your file system!

## For Users

### Installation

You can download the latest pre-compiled binary for your system from the [releases page](https://github.com/cauaeyer/deduplicate-cli/releases).

### Usage

To scan a directory, run the executable with the path to the directory as an argument:

```bash
./deduplicate /path/to/your/directory
```

If you don't provide a path, it will scan the current directory.

### Interactive Deletion

The program will guide you through the process of deleting duplicate files. For each group of identical files, you will be presented with the following options:

*   **`[1-n]`:** Keep the file with the corresponding number and delete the rest of the files in the group.
*   **`k`:** Keep all files in the current group.
*   **`d`:** Delete all files in the current group.
*   **`q`:** Quit the interactive session.

After reviewing all the groups, you will be asked for a final confirmation before any files are deleted.

## For Developers

### Prerequisites

*   **CMake:** A C++ compiler that supports C++20.
*   **OpenSSL:** The program depends on the OpenSSL library for hashing files.
*   **TBB (Threading Building Blocks):** Required for multi-threading support.
*   **oneDPL (oneAPI Data Parallel C++ Library):** Required for multi-threading support on macOS with Clang.
*   **GCC (on macOS):** Due to limitations in Apple's Clang compiler regarding C++ parallel execution, it is recommended to use GCC on macOS.

You can install the dependencies on a Debian-based system with:

```bash
sudo apt-get install build-essential cmake libssl-dev libtbb-dev
```

On macOS, you can use Homebrew:

```bash
brew install cmake openssl tbb onedpl gcc
```

### Building

1.  Clone the repository:

    ```bash
    git clone https://github.com/cauaeyer/deduplicate-cli.git
    cd deduplicate-cli
    ```

2.  Create a build directory:

    ```bash
    mkdir build
    ```

3.  Run CMake and build the project.

    **On Linux:**

    ```bash
    cmake -G Ninja -B build && ninja -C build
    ```

    **On macOS (with GCC):**

    ```bash
    CC=$(brew --prefix)/bin/gcc-15 CXX=$(brew --prefix)/bin/g++-15 cmake -G Ninja -B build && ninja -C build
    ```
    (Replace `gcc-15` and `g++-15` with your installed GCC version if different)

The executable will be created in the `build` directory with the name `deduplicate`.

### Some Commands:

To add to system's Path (or automatically install in any other path of your machine)

```bash
cmake --install build --component Binary --prefix <path>
```

To create zip file

```bash
cpack --config build/CPackConfig.cmake
```

## Contributing

Contributions are welcome! If you have any ideas, suggestions, or bug reports, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
