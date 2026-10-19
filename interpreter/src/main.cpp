#include "../include/emulator.hpp"

int main(int argc, char** argv)
{
    Emulator emulator = Emulator{};
    emulator.LoadROM("/home/mango/personal/CHIP-8-Emulator/binary.ch8");

    emulator.StartGame();

    return 0;
}

