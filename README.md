# CHIP-8 Emulator

This is a [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) emulator designed in C++.

## Build
The project requires the following prerequisites for a successful build.

- [SDL2](https://wiki.libsdl.org/SDL2/Installation#linuxunix) 
- [CMake](https://cmake.org/download/)

**NOTE**- The project is designed to be built for Linux/Unix systems. It can be built for Windows as well using the [WSL](https://learn.microsoft.com/en-us/windows/wsl/install)
environment. Please ensure that you install the Linux version of these dependencies when working with WSL.

From the project directory, use the following commands to build the executable
```shell
cmake -S . -B build
cd build
make
```

## Run

Use the following command to run the executable

```shell
./chip8 path/to/rom
```

The emulator comes with a *verbose* mode that will print the relevant details of each instruction that is executed. Use the `--v` flag to enable verbose mode.

```shell
./chip8 path/to/rom --v
```

## Controls

The emulator uses a hexadecimal keypad to enter inputs for programs that allow for it. The mapping is shown below.

```plaintext
 1 | 2 | 3 | C          1 | 2 | 3 | 4  
---+---+---+---        ---+---+---+---
 4 | 5 | 6 | D          Q | W | E | R
---+---+---+---   =>   ---+---+---+---
 7 | 8 | 9 | E          A | S | D | F 
---+---+---+---        ---+---+---+---
 A | 0 | B | F          Z | X | C | V
```
The `Esc` key can be pressed at anytime to immediately close the application.


## Credits

- [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [How to write an emulator (CHIP-8 interpreter)](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
- Timendus' [Chip-8 test suite](https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file)
- dmatlack's [repository of Chip-8 compatible programs](https://github.com/dmatlack/chip8) 