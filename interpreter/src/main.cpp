#include "../include/emulator.hpp"

int main(int argc, char** argv)
{
    // testing

    Emulator emulator = Emulator{};
    emulator.LoadROM("");
    emulator.StartGame();

    return 0;
}

