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

## Features

*   **Find duplicate files:** Scans a directory recursively to find files with identical content.
*   **Interactive deletion:** Provides an interactive interface to review groups of duplicate files and decide which ones to delete.
*   **Safe deletion:** Asks for final confirmation before deleting any files.
*   **Efficient:** First compares files by size and then by SHA256 hash to minimize I/O operations.

## Getting Started

### Prerequisites

*   **CMake:** A C++ compiler that supports C++20.
*   **OpenSSL:** The program depends on the OpenSSL library for hashing files.

You can install the dependencies on a Debian-based system with:

```bash
sudo apt-get install build-essential cmake libssl-dev
```

### Building

1.  Clone the repository:

    ```bash
    git clone https://github.com/your-username/nodupli.git
    cd nodupli
    ```

2.  Create a build directory:

    ```bash
    mkdir build
    cd build
    ```

3.  Run CMake and build the project:

    ```bash
    cmake ..
    make
    ```

The executable will be created in the `build` directory with the name `deduplicate-cmd`.

## Usage

To scan a directory, run the executable with the path to the directory as an argument:

```bash
./build/deduplicate-cmd /path/to/your/directory
```

If you don't provide a path, it will scan the current directory.

### Interactive Deletion

The program will guide you through the process of deleting duplicate files. For each group of identical files, you will be presented with the following options:

*   **`[1-n]`:** Keep the file with the corresponding number and delete the rest of the files in the group.
*   **`k`:** Keep all files in the current group.
*   **`d`:** Delete all files in the current group.
*   **`q`:** Quit the interactive session.

After reviewing all the groups, you will be asked for a final confirmation before any files are deleted.

## Contributing

Contributions are welcome! If you have any ideas, suggestions, or bug reports, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.