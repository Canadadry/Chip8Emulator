#ifndef _CHIP_8_H_
#define _CHIP_8_H_

#include <string>
#include <vector>


namespace Chip8 {

typedef unsigned short OpCode;
typedef unsigned short Address;
typedef unsigned char  Byte;

class CPU;

class Instruction{
public:
    Instruction(CPU& owner,std::string name,OpCode mask,OpCode id );
    const std::string name;
    const OpCode mask;
    const OpCode id;
    virtual void  action(OpCode code) const =0;
protected:
    CPU& m_owner;
    int getSubByte(int bytes, int num) const;
};

class Screen{
public:
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;

    enum Pixel{BLACK_PIXEL = 0 ,WHITE_PIXEL};
    virtual void  clearScreen() =0;
    virtual void  setPixel(int x, int y, Pixel p) =0;
    virtual Pixel getPixel(int x, int y) const =0;
};

class CPU
{
public:
    CPU(Screen* screen);

    bool loadROM(const unsigned char* rom, size_t size);
    void step();

    static const int MEMORY_SIZE    = 4096;
    static const int START_ADDRESS  = 512;
    static const int REGISTER_SIZE  = 16;
    static const int STACK_SIZE     = 16;
    static const int KEYBOARD_COUND = 16;

    // CPU description
    Byte memory[MEMORY_SIZE];
    Byte reg_V[REGISTER_SIZE];
    Address reg_I;
    Address stackAddress[STACK_SIZE];
    int stackCount;
    Byte synchronisationCounter;
    Byte soundCounter;
    Address pc;

    Screen* screen;
    bool    keyboard[KEYBOARD_COUND];


    bool drawByteAndCheckCollision(Byte x, Byte y, Byte b );
    void writeByteInMemoryAt(int byte, int address);
    void writeWordInMemoryAt(int msb, int lsb, int address);

private:
    std::vector<Instruction*> m_instructionSet;

    void fillInstructionSet();
    void initialize();
    void loadFont();

    OpCode getCurrentOpCode();
    Instruction* findInstruction(OpCode op);

    //optimization
    Instruction* memoryAsInstruction[MEMORY_SIZE>>1];
};
}

#endif // end of _CHIP_8_H_
