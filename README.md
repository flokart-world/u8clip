# u8clip

A command-line program for Windows to transfer standard input to the clipboard. Unlike clip.exe, it assumes that the encoding is UTF-8.

## How to use

```
$ cat your_file.txt | iconv -t utf8 | u8clip.exe
```

## How to build

Prerequisites:

- MSVC with C++17 support
- CMake 3.16.6 or later
- [CMakeSupports](https://github.com/flokart-world/cmake-supports) 0.0.7

Example:

```
$ cd /path/to/your/working/dir
$ git clone https://github.com/flokart-world/cmake-supports.git
$ git clone https://github.com/flokart-world/u8clip.git
$ mkdir build
$ cd build
$ cmake -D CMAKE_PREFIX_PATH=/path/to/your/working/dir/cmake-supports /path/to/your/working/dir/u8clip
$ cmake --build . --config Release
```
