# CHIP8 Emulator

Exactly what the title says: Another CHIP8 emulator in C++. It's the defacto emulation introduction for a reason! In order to build the project, run the following in command line:

## Desktop App

```bash
git clone https://github.com/mshettyFE/AnotherCHIP8.git
cd AnotherCHIP8
chmod +x setup.sh
./setup.sh
cd build # Make sure if you end up building for web as well, that you purge every file inside build first
cmake .. -DTESTING=False
make && ./CHIP8 ../roms/tetris.ch8
```

If for some reason, you want to run unit tests, run the following in the created build directory:

```bash
cmake .. -DTESTING=True
make && ./CHIP8 ../roms/tetris.ch8
```

NOTE: If you are on WSL, you will need to have some XServer program running to display the program.

## Web

If for some reason, you want to do this.

- Follow https://emscripten.org/docs/getting_started/downloads.html and install emsdk somewhere on your computer (could even be in this repo?)
- If you previously build the desktop version, you need to purge the ```build``` folder of every file before running these commands (ie ```rm -rf build```)
- Run the following commands in the CHIP8 ```build``` folder

```bash
emcmake cmake .. -DTESTING=False -DWEB=True
emmake make
```
