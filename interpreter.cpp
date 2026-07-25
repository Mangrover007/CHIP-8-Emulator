#include <cstdint>
#include <fstream>
#include <iostream>

const unsigned int FONTSIZE = 80;
uint8_t fontset[FONTSIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


class Chip8
{
private:
    const unsigned int START_ADDRESS = 0x200;
    const unsigned int FONTSET_START_ADDRESS = 0x50;

public:
    uint8_t  registers[16]{};
    uint8_t  memory[4096]{};

    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[16]{};
    uint8_t  sp{};

    uint8_t  delayTimer{};
    uint8_t  soundTimer{};

    uint8_t  keypad[16]{};
    uint32_t display[64 * 32]{};

    uint16_t opcode; // 2 byte opcode (ex: $C622)
    
    Chip8()
    {
	pc = START_ADDRESS;
    
	// put 0 to F into memory
	for (int i = 0; i < FONTSIZE; i++)
	{
	    memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}
    }
    
    void LoadROM(const char* filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (!file)
	{
	    std::cout << "UH oh, the ROM could not be opened!\n";
	    return;
	}

	// put the entire program in a buffer, then copy the contents of the
	// buffer into Chip8's memory

	int size = file.tellg();

	file.seekg(0, std::ios::beg);

	char *buf = new char[size];
	file.read(buf, size);
	file.close();

	for (int i = 0; i < size; i++)
	{
	    memory[START_ADDRESS + i] = buf[i];
	}

	delete[] buf;
    }
};

