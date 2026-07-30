#include "interpreter.hpp"
#include <fstream>
#include <iostream>

Chip8::Chip8() : randEngine(std::chrono::system_clock::to_time_t(time))
    {
	pc = START_ADDRESS;
	sp = -1;

	// put 0 to F into memory
	for (int i = 0; i < FONTSIZE; i++)
	{
	    memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	// initialize randEngine and rand
	rand = std::uniform_int_distribution<uint8_t>(0, 255U);
    }

void Chip8::LoadROM(const char* filename) {
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

// ----- CHIP-8 instruction set definitions ----- //
#include <string.h>

void Chip8::OP_00E0()
{
    memset(display, 0, sizeof(display));
}

void Chip8::OP_00EE()
{
    pc = stack[sp];
    sp -= 1;
}

void Chip8::OP_1nnn()
{
    uint16_t address = opcode & 0x0FFFu;
    pc = address;
}

void Chip8::OP_2nnn()
{
    uint16_t address = opcode & 0x0FFFu;
    sp += 1;
    stack[sp] = pc;
    pc = address;
}

void Chip8::OP_3xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    if (registers[Vx] == byte)
    {
	pc += 2;
    }
}

void Chip8::OP_4xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    if (registers[Vx] != byte)
    {
	pc += 2;
    }
}

void Chip8::OP_5xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    if (registers[Vx] == registers[Vy])
    {
	pc += 2;
    }
}

void Chip8::OP_6xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    registers[Vx] = byte;
}

void Chip8::OP_7xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    registers[Vx] += byte;
}

void Chip8::OP_8xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] = registers[Vy];
}

void Chip8::OP_8xy1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] |= registers[Vy];
}

void Chip8::OP_8xy2()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] &= registers[Vy];
}

void Chip8::OP_8xy3()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] ^= registers[Vy];
}

void Chip8::OP_8xy4()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    // set VF = 1 if the carry bit remains
    if (registers[Vx] + registers[Vy] > 255)
    {
	registers[0xF] = 1;
    }
    
    registers[Vx] += registers[Vy];
}

void Chip8::OP_8xy5()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    
    // set the borrow bit 1 if Vx > Vy
    if (registers[Vx] > registers[Vy])
    {
	registers[0xF] = 1;
    }

    registers[Vx] -= registers[Vy];
}

void Chip8::OP_8xy6()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    // set VF = 1 if least-significant-bit of Vy is 1
    registers[0xF] = Vy & 0x1u;

    registers[Vx] = registers[Vy] >> 1;
}

void Chip8::OP_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    // set VF = 1 if Vy > Vx
    if (registers[Vy] > registers[Vx])
    {
	registers[0xF] = 1;
    }

    registers[Vx] = registers[Vy] - registers[Vx];
}

void Chip8::OP_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    // set VF = 1 if most-significant-bit of Vy is 1
    registers[0xF] = Vy & 0x10u;

    registers[Vx] = registers[Vy] << 1;
}

void Chip8::OP_9xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] != registers[Vy])
    {
	pc += 2;
    }
}

void Chip8::OP_Annn()
{
    uint16_t addr = opcode & 0x0FFFu;
    index = addr;
}

void Chip8::OP_Bnnn()
{
    uint16_t addr = opcode & 0x0FFFu;
    pc = addr + registers[0];
}

void Chip8::OP_Cxkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t mask = opcode & 0x00FFu;
    uint8_t randNum = Chip8::rand(Chip8::randEngine);
    registers[Vx] = randNum & mask;
}

void Chip8::OP_Dxyn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu;

    registers[0xF] = 0; // optimistic case for no collision

    for (unsigned int row = 0; row < height; row++)
    {
	uint8_t sprite_row = memory[index + row];

	for (unsigned int col = 0; col < 8; col++)
	{
	    // pixel data (0 or 1)
	    uint8_t pixel_data = sprite_row & (0x80u >> 1);
	    
	    // final position of the pixel on display
	    // skip Vy + i rows of col, then from Vx, fill pixel data
	    uint8_t yPos = ((registers[Vy] + row) % DISPLAY_HEIGHT) * DISPLAY_WIDTH;
	    uint8_t xPos = (registers[Vx] + col) % DISPLAY_WIDTH;

	    if (pixel_data == 1)
	    {
		if (display[xPos + yPos] == 0xFFFFFFFF)
		{
		    registers[0xF] = 1;
		}

		display[xPos + yPos] ^= 0xFFFFFFFF;
	    }
	}
    }
}

