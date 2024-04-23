 Exactly what the title says: Another CHIP8 emulator in C++. It's the defacto emulation introduction for a reason! In order to build the project, run the following in command line:

* git clone https://github.com/mshettyFE/AnotherCHIP8.git
* cd AnotherCHIP8
* chmod +x setup.sh
* ./setup.sh
* cd build
* cmake .. -DTESTING=False
* make && ./CHIP8 ../roms/tetris.ch8

If for some reason, you want to run unit tests, run the following in the created build directory:
* cmake .. -DTESTING=True
* make && ./CHIP8 ../roms/tetris.ch8

NOTE: If you are on WSL, you will need to have some XServer program running to display the program.
