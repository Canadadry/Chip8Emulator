#include "chip8.h"
#include <cstdio>

namespace Chip8 {

const int Screen::SCREEN_WIDTH  = 64;
const int Screen::SCREEN_HEIGHT  = 32;



Instruction::Instruction(CPU& owner,std::string p_name,OpCode p_mask,OpCode p_id )
    :name(p_name)
    ,mask(p_mask)
    ,id(p_id)
    , m_owner(owner)
{}

int Instruction::getSubByte(int bytes, int num) const
{
    return (bytes & ((0xF)<<(num*4)))>>(num*4);
}


class Instruction0NNN : public Instruction
{
public:
    Instruction0NNN(CPU& owner) :Instruction(owner,"0NNN",0xF000,0x0000){}
    virtual void  action(OpCode code) const{}
};

class Instruction00E0 : public Instruction
{
public:
    Instruction00E0(CPU& owner) :Instruction(owner,"00E0",0xFFFF,0x00E0){}
    virtual void  action(OpCode code) const{
	m_owner.screen->clearScreen();
    }
};

class Instruction00EE : public Instruction
{
public:
    Instruction00EE(CPU& owner) :Instruction(owner,"00EE",0xFFFF,0x00EE){}
    virtual void  action(OpCode code) const{
	if(m_owner.stackCount > 0)
	{
	    m_owner.pc = m_owner.stackAddress[m_owner.stackCount];
	    m_owner.stackCount--;
	}
    }
};

class Instruction1NNN : public Instruction
{
public:
    Instruction1NNN(CPU& owner) :Instruction(owner,"1NNN",0xF000,0x1000){}
    virtual void  action(OpCode code) const{
	m_owner.pc = (code&0x0FFF);
    }
};

class Instruction2NNN : public Instruction
{
public:
    Instruction2NNN(CPU& owner) :Instruction(owner,"2NNN",0xF000,0x2000){}
    virtual void  action(OpCode code) const{
	if(m_owner.stackCount>=(CPU::STACK_SIZE-1))
	{
	    //System.out.println("stackOverFlow at : "+ programCounter + "\n");
	}
	else
	{
	    m_owner.stackCount++;
	    m_owner.stackAddress[m_owner.stackCount] = m_owner.pc;
	}
	m_owner.pc = (code&0x0FFF);
    }
};

class Instruction3XNN : public Instruction
{
public:
    Instruction3XNN(CPU& owner) :Instruction(owner,"3XNN",0xF000,0x3000){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int NN = 	   code & 0XFF;
	if(m_owner.reg_V[X] == NN)
	{
	    m_owner.pc+=2;
	}
    }
};

class Instruction4XNN : public Instruction
{
public:
    Instruction4XNN(CPU& owner) :Instruction(owner,"4XNN",0xF000,0x4000){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int NN = 	   code & 0XFF;
	if(m_owner.reg_V[X] != NN)
	{
	    m_owner.pc+=2;
	}
    }
};

class Instruction5XY0 : public Instruction
{
public:
    Instruction5XY0(CPU& owner) :Instruction(owner,"5XY0",0xF00F,0x5000){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	if(m_owner.reg_V[X] == m_owner.reg_V[Y])
	{
	    m_owner.pc+=2;
	}
    }
};

class Instruction6XNN : public Instruction
{
public:
    Instruction6XNN(CPU& owner) :Instruction(owner,"6XNN",0xF000,0x6000){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int NN = 	   code & 0XFF;
	m_owner.reg_V[X] = NN;
    }
};

class Instruction7XNN : public Instruction
{
public:
    Instruction7XNN(CPU& owner) :Instruction(owner,"7XNN",0xF000,0x7000){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int NN = 	   code & 0XFF;
	m_owner.reg_V[X] += NN;
    }
};

class Instruction8XY0 : public Instruction
{
public:
    Instruction8XY0(CPU& owner) :Instruction(owner,"8XY0",0xF00F,0x8000){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	m_owner.reg_V[X] = m_owner.reg_V[Y];
    }
};

class Instruction8XY1 : public Instruction
{
public:
    Instruction8XY1(CPU& owner) :Instruction(owner,"8XY1",0xF00F,0x8001){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	m_owner.reg_V[X] = m_owner.reg_V[X] | m_owner.reg_V[Y];
    }
};

class Instruction8XY2 : public Instruction
{
public:
    Instruction8XY2(CPU& owner) :Instruction(owner,"8XY2",0xF00F,0x8002){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	m_owner.reg_V[X] = m_owner.reg_V[X] & m_owner.reg_V[Y];
    }
};

class Instruction8XY3 : public Instruction
{
public:
    Instruction8XY3(CPU& owner) :Instruction(owner,"8XY3",0xF00F,0x8003){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	m_owner.reg_V[X] = m_owner.reg_V[X] ^ m_owner.reg_V[Y];
    }
};

class Instruction8XY4 : public Instruction
{
public:
    Instruction8XY4(CPU& owner) :Instruction(owner,"8XY4",0xF00F,0x8004){}
    virtual void  action(OpCode code) const
    {
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	m_owner.reg_V[X] = m_owner.reg_V[X] + m_owner.reg_V[Y];
	m_owner.reg_V[0xf] = (m_owner.reg_V[X] + m_owner.reg_V[Y]) > 0xFF ? 1 : 0;
    }
};

class Instruction8XY5 : public Instruction
{
public:
    Instruction8XY5(CPU& owner) :Instruction(owner,"8XY5",0xF00F,0x8005){}
    virtual void  action(OpCode code) const
    {
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	m_owner.reg_V[X] = m_owner.reg_V[X] - m_owner.reg_V[Y];
	m_owner.reg_V[0xf] = (m_owner.reg_V[X] < m_owner.reg_V[Y])  ? 1 : 0;
    }
};

class Instruction8XY6 : public Instruction
{
public:
    Instruction8XY6(CPU& owner) :Instruction(owner,"8XY6",0xF00F,0x8006){}
    virtual void  action(OpCode code) const
    {
	int X = 	   getSubByte(code, 2) ;

	m_owner.reg_V[0xf] = m_owner.reg_V[X] &0x1;
	m_owner.reg_V[X]   = m_owner.reg_V[X] >> 1;
    }
};

class Instruction8XY7 : public Instruction
{
public:
    Instruction8XY7(CPU& owner) :Instruction(owner,"8XY7",0xF00F,0x8007){}
    virtual void  action(OpCode code) const
    {
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	m_owner.reg_V[X] = m_owner.reg_V[Y] - m_owner.reg_V[X];
	m_owner.reg_V[0xf] = (m_owner.reg_V[X] > m_owner.reg_V[Y])  ? 1 : 0;
    }
};

class Instruction8XYE : public Instruction
{
public:
    Instruction8XYE(CPU& owner) :Instruction(owner,"8XYE",0xF00F,0x800E){}
    virtual void  action(OpCode code) const
    {
	int X = 	   getSubByte(code, 2) ;

	m_owner.reg_V[0xf] = m_owner.reg_V[X]>>7 &0x1;
	m_owner.reg_V[X]   = m_owner.reg_V[X] << 1;
    }
};

class Instruction9XY0 : public Instruction
{
public:
    Instruction9XY0(CPU& owner) :Instruction(owner,"9XY0",0xF00F,0x9000){}
    virtual void  action(OpCode code) const
    {
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	if(m_owner.reg_V[Y] != m_owner.reg_V[X])
	{
	    m_owner.pc+=2;
	}
    }
};

class InstructionANNN : public Instruction
{
public:
    InstructionANNN(CPU& owner) :Instruction(owner,"ANNN",0xF000,0xA000){}
    virtual void  action(OpCode code) const{
	m_owner.reg_I = code & 0xFFF;
    }
};

class InstructionBNNN : public Instruction
{
public:
    InstructionBNNN(CPU& owner) :Instruction(owner,"BNNN",0xF000,0xB000){}
    virtual void  action(OpCode code) const{
	m_owner.pc = code & 0xFFF + m_owner.reg_V[0];
    }
};

class InstructionCXNN : public Instruction
{
public:
    InstructionCXNN(CPU& owner) :Instruction(owner,"CXNN",0xF000,0xC000){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	m_owner.reg_V[X]=rand()%(code&0xFF);
    }
};

class InstructionDXYN : public Instruction
{
public:
    InstructionDXYN(CPU& owner) :Instruction(owner,"DXYN",0xF000,0xD000){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	int Y = 	   getSubByte(code, 1) ;
	int N = 	   getSubByte(code, 0) ;

	bool collide = false;
	for(int i = 0;i<N;i++)
	{
	    collide = collide | m_owner.drawByteAndCheckCollision(m_owner.reg_V[X], m_owner.reg_V[Y]+i, m_owner.memory[m_owner.reg_I+i]);
	}
    }
};

class InstructionEX9E : public Instruction
{
public:
    InstructionEX9E(CPU& owner) :Instruction(owner,"EX9E",0xF0FF,0xE0E9){}
    virtual void  action(OpCode code) const
    {
	int X = 	   getSubByte(code, 2) ;
	if(m_owner.reg_V[X] < CPU::KEYBOARD_COUND)
	{
	    if(m_owner.keyboard[m_owner.reg_V[X]] == true)
	    {
		m_owner.pc+=2;
	    }
	}
    }
};

class InstructionEXA1 : public Instruction
{
public:
    InstructionEXA1(CPU& owner) :Instruction(owner,"EXA1",0xF0FF,0xE0A1){}
    virtual void  action(OpCode code) const
    {
	int X = 	   getSubByte(code, 2) ;
	if(m_owner.reg_V[X] < CPU::KEYBOARD_COUND)
	{
	    if(m_owner.keyboard[m_owner.reg_V[X]] == false)
	    {
		m_owner.pc+=2;
	    }
	}
    }
};

class InstructionFX07 : public Instruction
{
public:
    InstructionFX07(CPU& owner) :Instruction(owner,"FX07",0xF0FF,0xF007){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2);
	m_owner.reg_V[X] = m_owner.synchronisationCounter;
    }
};

class InstructionFXA0 : public Instruction
{
public:
    InstructionFXA0(CPU& owner) :Instruction(owner,"FXA0",0xF0FF,0xF0A0){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2);
	bool keyPressed = false;
	int keyID = -1;
	for(int i=0;i<CPU::KEYBOARD_COUND;i++)
	{
	    keyPressed = keyPressed || m_owner.keyboard[i];
	    if(m_owner.keyboard[i] )
	    {
		keyID = i;
		break;
	    }
	}
	if(keyPressed == false) m_owner.pc -= 2;
	else{
	    m_owner.reg_V[X] = keyID;
	}
    }
};

class InstructionFX15 : public Instruction
{
public:
    InstructionFX15(CPU& owner) :Instruction(owner,"FX15",0xF0FF,0xF015){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2);
	m_owner.synchronisationCounter = m_owner.reg_V[X];
    }
};

class InstructionFX18 : public Instruction
{
public:
    InstructionFX18(CPU& owner) :Instruction(owner,"FX18",0xF0FF,0xF018){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2);
	m_owner.soundCounter = m_owner.reg_V[X];
    }
};

class InstructionFX1E : public Instruction
{
public:
    InstructionFX1E(CPU& owner) :Instruction(owner,"FX1E",0xF0FF,0xF01E){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	m_owner.reg_V[0xF] = ((m_owner.reg_V[X] + m_owner.reg_I) > 0xFF) ? 1 : 0;
	m_owner.reg_I += m_owner.reg_V[X] ;
    }
};

class InstructionFX29 : public Instruction
{
public:
    InstructionFX29(CPU& owner) :Instruction(owner,"FX29",0xF0FF,0xF029){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	m_owner.reg_I = m_owner.reg_V[X]*5 ;
    }
};

class InstructionFX33 : public Instruction
{
public:
    InstructionFX33(CPU& owner) :Instruction(owner,"FX33",0xF0FF,0xF033){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	m_owner.memory[m_owner.reg_I+0] = (short) (m_owner.reg_V[X]/100) ;
	m_owner.memory[m_owner.reg_I+1] = (short) ((int)(m_owner.reg_V[X]/10 ) - (int)(m_owner.reg_V[X]/100)*10)  ;
	m_owner.memory[m_owner.reg_I+2] = (short) ((m_owner.reg_V[X]) - (int)(m_owner.reg_V[X]/10)*10)  ;
    }
};

class InstructionFX55 : public Instruction
{
public:
    InstructionFX55(CPU& owner) :Instruction(owner,"FX55",0xF0FF,0xF055){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	memcpy(&m_owner.memory[m_owner.reg_I], m_owner.reg_V,X*sizeof(Byte));
    }
};

class InstructionFX65 : public Instruction
{
public:
    InstructionFX65(CPU& owner) :Instruction(owner,"FX65",0xF0FF,0xF065){}
    virtual void  action(OpCode code) const{
	int X = 	   getSubByte(code, 2) ;
	memcpy(m_owner.reg_V,&m_owner.memory[m_owner.reg_I],X*sizeof(Byte));
    }
};

class InstructionNOP : public Instruction
{
public:
    InstructionNOP(CPU& owner) :Instruction(owner,"NOP",0x0000,0x0000){}
    virtual void  action(OpCode code) const{}
};


CPU::CPU(Screen* p_screen)
    : memory()
    , reg_V()
    , reg_I(0)
    , stackAddress()
    , stackCount(0)
    , synchronisationCounter(0)
    , soundCounter(0)
    , pc(START_ADDRESS)
    , screen(p_screen)
    , keyboard()
{
    initialize();
    fillInstructionSet();
}

void CPU::initialize()
{
    memset(memory,0,MEMORY_SIZE*sizeof(Byte));
    memset(reg_V,0,REGISTER_SIZE*sizeof(Byte));
    reg_I = 0;
    memset(stackAddress,0,STACK_SIZE*sizeof(Address));
    stackCount =0;
    synchronisationCounter = 0;
    soundCounter = 0;
    pc= START_ADDRESS;
    screen->clearScreen();
    memset(keyboard,0,sizeof(keyboard));
    loadFont();

}

bool CPU::loadROM(const unsigned char* rom, size_t size)
{
    initialize();

    if(size < MEMORY_SIZE -START_ADDRESS)
    {
	memcpy(&memory[START_ADDRESS],rom,size);
	return true;
    }
    return false;
}

void CPU::step()
{
    OpCode op = getCurrentOpCode();
    pc+=2;
    Instruction* instruction = findInstruction(op);
    //printf("excuting opcode %x from @ %x interpreted has %s\n",op,pc-2,instruction->name.c_str());
    //fflush(stdout);
    instruction->action(op);
    soundCounter --;
    if(soundCounter <0 )soundCounter=0;
    synchronisationCounter --;
    if(synchronisationCounter <0 )synchronisationCounter=0;
    if(pc>= MEMORY_SIZE) pc = START_ADDRESS;
}

OpCode CPU::getCurrentOpCode()
{
    return (memory[pc]<<8)+memory[pc+1];
}

Instruction* CPU::findInstruction(OpCode op)
{
    std::vector<Instruction*>::iterator it ;
    for(it = m_instructionSet.begin();it != m_instructionSet.end(); it++)
    {
	//printf("checking instruction %s op %x mask %x id %x\n",(*it)->name.c_str(),op , (*it)->mask,(*it)->id);
	if((op & (*it)->mask) == (*it)->id)
	{
	  //  printf("found it ^^\n");
	    return *it;
	}
    }

    return m_instructionSet.back();
}


bool CPU::drawByteAndCheckCollision(Byte x, Byte y, Byte b )
{
    bool ret = false;
    for(int i = 0; i<8;i++)
    {
	if(x+i >= Screen::SCREEN_WIDTH) return ret;
	Screen::Pixel p = (b>>i & 1) ==1 ? Screen::WHITE_PIXEL : Screen::BLACK_PIXEL;
	if(p == Screen::WHITE_PIXEL)
	{
	    if(screen->getPixel(x+i,y) == Screen::WHITE_PIXEL)
	    {
		screen->setPixel(x+i,y,Screen::BLACK_PIXEL);
		ret= true;
	    }
	    else
	    {
		screen->setPixel(x+i,y,Screen::WHITE_PIXEL);
	    }
	}
    }
    return ret;
}


void CPU::fillInstructionSet()
{
    m_instructionSet.push_back(new Instruction0NNN(*this));
    m_instructionSet.push_back(new Instruction00E0(*this));
    m_instructionSet.push_back(new Instruction00EE(*this));
    m_instructionSet.push_back(new Instruction1NNN(*this));
    m_instructionSet.push_back(new Instruction2NNN(*this));
    m_instructionSet.push_back(new Instruction3XNN(*this));
    m_instructionSet.push_back(new Instruction4XNN(*this));
    m_instructionSet.push_back(new Instruction5XY0(*this));
    m_instructionSet.push_back(new Instruction6XNN(*this));
    m_instructionSet.push_back(new Instruction7XNN(*this));
    m_instructionSet.push_back(new Instruction8XY0(*this));
    m_instructionSet.push_back(new Instruction8XY1(*this));
    m_instructionSet.push_back(new Instruction8XY2(*this));
    m_instructionSet.push_back(new Instruction8XY3(*this));
    m_instructionSet.push_back(new Instruction8XY4(*this));
    m_instructionSet.push_back(new Instruction8XY5(*this));
    m_instructionSet.push_back(new Instruction8XY6(*this));
    m_instructionSet.push_back(new Instruction8XY7(*this));
    m_instructionSet.push_back(new Instruction8XYE(*this));
    m_instructionSet.push_back(new Instruction9XY0(*this));
    m_instructionSet.push_back(new InstructionANNN(*this));
    m_instructionSet.push_back(new InstructionBNNN(*this));
    m_instructionSet.push_back(new InstructionCXNN(*this));
    m_instructionSet.push_back(new InstructionDXYN(*this));
    m_instructionSet.push_back(new InstructionEX9E(*this));
    m_instructionSet.push_back(new InstructionEXA1(*this));
    m_instructionSet.push_back(new InstructionFX07(*this));
    m_instructionSet.push_back(new InstructionFXA0(*this));
    m_instructionSet.push_back(new InstructionFX15(*this));
    m_instructionSet.push_back(new InstructionFX18(*this));
    m_instructionSet.push_back(new InstructionFX1E(*this));
    m_instructionSet.push_back(new InstructionFX29(*this));
    m_instructionSet.push_back(new InstructionFX33(*this));
    m_instructionSet.push_back(new InstructionFX55(*this));
    m_instructionSet.push_back(new InstructionFX65(*this));
    m_instructionSet.push_back(new InstructionNOP (*this));
}

void CPU::loadFont()
{
    int i = 0x0;

    //0 SPRITE
    memory[i++] = 0b01111110;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11000011;
    memory[i++] = 0b11000011;
    memory[i++] = 0b11000011;
    memory[i++] = 0b11000011;
    memory[i++] = 0b11000011;
    memory[i++] = 0b11000011;
    memory[i++] = 0b11111111;
    memory[i++] = 0b01111110;
    //1 SPRITE
    memory[i++] = 0b00111000;
    memory[i++] = 0b11111000;
    memory[i++] = 0b11111000;
    memory[i++] = 0b00111000;
    memory[i++] = 0b00111000;
    memory[i++] = 0b00111000;
    memory[i++] = 0b00111000;
    memory[i++] = 0b00111000;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11111111;
    //2 SPRITE
    memory[i++] = 0b00111100;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b00000111;
    memory[i++] = 0b00001110;
    memory[i++] = 0b00011100;
    memory[i++] = 0b00111000;
    memory[i++] = 0b01110000;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11111111;
    //3 SPRITE
    memory[i++] = 0b00111100;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b00000111;
    memory[i++] = 0b01111110;
    memory[i++] = 0b01111110;
    memory[i++] = 0b00001111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b00111100;
    //4 SPRITE
    memory[i++] = 0b00011110;
    memory[i++] = 0b00111110;
    memory[i++] = 0b11100110;
    memory[i++] = 0b11000110;
    memory[i++] = 0b11111111;
    memory[i++] = 0b01111111;
    memory[i++] = 0b00001110;
    memory[i++] = 0b00001110;
    memory[i++] = 0b00001110;
    memory[i++] = 0b00001110;
    //5 SPRITE
    memory[i++] = 0b11111111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100000;
    memory[i++] = 0b01111000;
    memory[i++] = 0b00111100;
    memory[i++] = 0b00001111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b01111110;
    //6 SPRITE
    memory[i++] = 0b00001111;
    memory[i++] = 0b00111100;
    memory[i++] = 0b01111000;
    memory[i++] = 0b11110000;
    memory[i++] = 0b11111110;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b01111110;
    //7 SPRITE
    memory[i++] = 0b11111111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b00001110;
    memory[i++] = 0b00011100;
    memory[i++] = 0b00111000;
    memory[i++] = 0b00111000;
    memory[i++] = 0b00111000;
    memory[i++] = 0b00111000;
    memory[i++] = 0b00111000;
    //8 SPRITE
    memory[i++] = 0b00111100;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11000011;
    memory[i++] = 0b11100111;
    memory[i++] = 0b00111100;
    memory[i++] = 0b00111100;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11000011;
    memory[i++] = 0b11100111;
    memory[i++] = 0b00111100;
    //9 SPRITE
    memory[i++] = 0b01111110;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b01111111;
    memory[i++] = 0b00001111;
    memory[i++] = 0b00011110;
    memory[i++] = 0b0011110;
    memory[i++] = 0b1111000;
    //A SPRITE
    memory[i++] = 0b00111100;
    memory[i++] = 0b01100110;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11100111;
    //B SPRITE
    memory[i++] = 0b11111110;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11111110;
    memory[i++] = 0b11111110;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11111110;
    //C SPRITE
    memory[i++] = 0b01111110;
    memory[i++] = 0b11100111;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100111;
    memory[i++] = 0b01111110;
    //D SPRITE
    memory[i++] = 0b11111100;
    memory[i++] = 0b11100110;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100011;
    memory[i++] = 0b11100110;
    memory[i++] = 0b11111100;
    //E SPRITE
    memory[i++] = 0b11111111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11100001;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11111110;
    memory[i++] = 0b11111110;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100001;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11111111;
    //F SPRITE
    memory[i++] = 0b11111111;
    memory[i++] = 0b11111111;
    memory[i++] = 0b11100001;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11111110;
    memory[i++] = 0b11111110;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100000;
    memory[i++] = 0b11100000;
}

}

