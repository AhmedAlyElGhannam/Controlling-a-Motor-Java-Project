# ITI_Embedded_AVR_Playground

## Compiling, Building, and Flashing
```bash
cmake -S . -B build 
cmake --build build
cd build
make flash
```

## Simulating Using SimulAVR && Debugging Using GDB

> Open a terminal and execute the following:

```bash
simulavr -d atmega32 -f AVR_PROJECT.elf --gdbserver
```

> In another terminal, run:

```bash
avr-gdb AVR_PROJECT.elf
```

> In order to begin debugging using GDB, connect it with SimulAVR using the following command. Port 1212 is the default port for SimulAVR; make sure it matches the port returned by SimulAVR upon starting it.

```bash
target remote localhost:1212
```

> Symbol Table is read by GDB by default if `-g` compiler flag was used to add debugging symbols.

### Common GDB Commands

1. To place a breakpoint:
   ```bash
   break LABEL_NAME
   ```

1. To continue executing until breakpoint is reached:
   ```bash
   c
   ```

1. To print a variable's value (if it is a pointer for example):
   ```bash
   print *PTR_NAME
   ```

1. To single step in C code:
   ```bash
   n
   ```

1. To switch code layout from C to Assembly:
   ```bash
   layout asm
   ```

1. To single step in Assembly code:
    ```bash
    stepi
    ```
    ```bash
    si
    ```

1. To switch code layout back to C:
   ```bash
   layout src
   ```

## Simulating Using SimulAVR && Debugging Using GDB Through VSCode

