#include <cstdint>
#include <random>
#include <chrono>

const unsigned int FONTSIZE = 80;

const uint8_t fontset[FONTSIZE] = {
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

public:
    Chip8();
    void LoadROM(const char * filename);

private:
    const unsigned int START_ADDRESS = 0x200;
    const unsigned int FONTSET_START_ADDRESS = 0x50;

    const std::chrono::system_clock::time_point time = std::chrono::system_clock::now();

public:
    const uint8_t DISPLAY_HEIGHT = 32;
    const uint8_t DISPLAY_WIDTH = 64;

public:
    std::default_random_engine randEngine;
    std::uniform_int_distribution<uint8_t> rand;

// All 36 CHIP-8 instructions for simulation
public:
    void OP_0nnn();
    void OP_00E0(); // CLS
    void OP_00EE(); // RET
    
    void OP_1nnn(); // JMP addr
    
    void OP_2nnn(); // CALL addr
    
    void OP_3xkk(); // SE Vx, byte
    
    void OP_4xkk(); // SNE Vx, byte
    
    void OP_5xy0(); // SE Vx, Vy
    
    void OP_6xkk(); // LD Vx, byte

    void OP_7xkk(); // ADD Vx, byte

    void OP_8xy0(); // LD Vx, Vy
    void OP_8xy1(); // OR Vx, Vy
    void OP_8xy2(); // AND Vx, Vy
    void OP_8xy3(); // XOR Vx, Vy
    void OP_8xy4(); // ADD Vx, Vy; VF = 1 if result > 255
    void OP_8xy5(); // SUB Vx, Vy; (Vx - Vy) VF = 1 if Vx > Vy
    void OP_8xy6(); // SHR Vx, Vy; VF = least-significant-bit in Vy
    void OP_8xy7(); // SUBN Vx, Vy; (Vy - Vx) VF = 1 if Vy > Vx
    void OP_8xyE(); // SHL Vx, Vy; VF = most-significant-bit in Vy

    void OP_9xy0(); // SNE Vx, Vy
    
    void OP_Annn(); // LD I, nnn

    void OP_Bnnn(); // JMP V0, addr

    void OP_Cxkk(); // RND, Vx, mask

    void OP_Dxyn(); // DRW Vx, Vy, height
};

