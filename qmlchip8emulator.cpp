#include "qmlchip8emulator.h"
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QQuickWindow>

QMLChip8Emulator::QMLChip8Emulator(QQuickPaintedItem *parent)
    : QQuickPaintedItem(parent)
    , m_screen(QSize(Chip8::Screen::SCREEN_WIDTH,Chip8::Screen::SCREEN_HEIGHT),QImage::Format_RGB32)
    , m_cpu(this)
    , m_need_update(true)
    , m_run_cpu_cycle(0)

{
    m_screen.fill(QColor::fromRgb(0,0,0));
    cycle();
}

void QMLChip8Emulator::paint(QPainter * painter)
{
    painter->drawImage(QRect(0,0,width(),height()),m_screen,QRect(0,0,Chip8::Screen::SCREEN_WIDTH,Chip8::Screen::SCREEN_HEIGHT));
}

void QMLChip8Emulator::cycle()
{
    for(int i=0;i<m_run_cpu_cycle;i++) m_cpu.step();
    if(m_need_update) update();
    m_need_update= false;

    QTimer::singleShot(20,this,SLOT(cycle()));
}

void  QMLChip8Emulator::clearScreen()
{
    m_screen.fill(0);
    m_need_update = true;
}

void  QMLChip8Emulator::setPixel(int x, int y, Pixel p)
{
    // qDebug() << "setting pixel at ( " << x <<","<<y<<") of color " << ((p==Chip8::Screen::WHITE_PIXEL)?"White":"Black");
    m_screen.setPixel(x,y,p==Chip8::Screen::WHITE_PIXEL?qRgb(255,255,255):qRgb(0,0,0));
    m_need_update = true;
}

Chip8::Screen::Pixel QMLChip8Emulator::getPixel(int x, int y) const
{
    return m_screen.pixel(x,y) == qRgb(255,255,255) ? Chip8::Screen::WHITE_PIXEL : Chip8::Screen::BLACK_PIXEL;
}

void QMLChip8Emulator::play(int countByFrame)
{
    if(   m_run_cpu_cycle != countByFrame )
    {
	m_run_cpu_cycle = countByFrame;
	emit runningChanged();
	emitCPUChanged();
    }
}

void QMLChip8Emulator::pause()
{
    play(0);
}


void QMLChip8Emulator::step()
{
    m_cpu.step();
    emitCPUChanged();
}

void QMLChip8Emulator::loadROM()
{
    QString fileName ;//= QFileDialog::getOpenFileName(0,"Open Rom",QDir::home().path());
    fileName = "/Users/mooglwy/Desktop/MAZE.ch8";
    FILE* fd = fopen(fileName.toStdString().c_str(),"r");

    //    qDebug() << "trying to open " << QDir::home().path();
    if(fd != NULL)
    {
	//	qDebug() << "opening ";

	char data[Chip8::CPU::MEMORY_SIZE];
	int size = fread(data,1,Chip8::CPU::MEMORY_SIZE,fd);
	//	qDebug() << "read " << size;
	if(size>0)
	{
	    m_cpu.loadROM((const unsigned char*)data,size);
	}
    }

}

int QMLChip8Emulator::readMemoryAt(int address)
{
    if( address >= 0 && address < Chip8::CPU::MEMORY_SIZE) return m_cpu.memory[address];
    return 0;
}


int QMLChip8Emulator::readWordMemoryAt(int address)
{
    return (readMemoryAt(address) << 8) + readMemoryAt(address+1);
}

void QMLChip8Emulator::writeByteInMemoryAt(int byte, int address)
{
    if( address >= 0 && address < Chip8::CPU::MEMORY_SIZE) m_cpu.writeByteInMemoryAt(byte,address);
}

void QMLChip8Emulator::writeWordInMemoryAt(int word, int address)
{
    writeByteInMemoryAt((word & 0xFF00)>> 8,address);
    writeByteInMemoryAt(word & 0xFF,address+1);
}

void QMLChip8Emulator::emitCPUChanged()
{
    emit opChanged();
    emit pcChanged();
    emit iChanged();
    emit syncCounterChanged();
    emit soundCounterChanged();
    emit stackPointerChanged();
    emit v0Changed();
    emit v1Changed();
    emit v2Changed();
    emit v3Changed();
    emit v4Changed();
    emit v5Changed();
    emit v6Changed();
    emit v7Changed();
    emit v8Changed();
    emit v9Changed();
    emit vAChanged();
    emit vBChanged();
    emit vCChanged();
    emit vDChanged();
    emit vEChanged();
    emit vFChanged();
    emit stack0Changed();
    emit stack1Changed();
    emit stack2Changed();
    emit stack3Changed();
    emit stack4Changed();
    emit stack5Changed();
    emit stack6Changed();
    emit stack7Changed();
    emit stack8Changed();
    emit stack9Changed();
    emit stackAChanged();
    emit stackBChanged();
    emit stackCChanged();
    emit stackDChanged();
    emit stackEChanged();
    emit stackFChanged();
}

bool QMLChip8Emulator::running()    const { return (m_run_cpu_cycle != 0);}

int QMLChip8Emulator::op()           const { return (m_cpu.memory[m_cpu.pc]<<8)+m_cpu.memory[m_cpu.pc+1]; }
int QMLChip8Emulator::pc()           const { return m_cpu.pc;           }
int QMLChip8Emulator::i()            const { return m_cpu.reg_I;            }
int QMLChip8Emulator::syncCounter()  const { return m_cpu.synchronisationCounter;  }
int QMLChip8Emulator::soundCounter() const { return m_cpu.soundCounter; }
int QMLChip8Emulator::stackPointer() const { return m_cpu.stackCount; }
int QMLChip8Emulator::v0()           const { return m_cpu.reg_V[0x0]; }
int QMLChip8Emulator::v1()           const { return m_cpu.reg_V[0x1]; }
int QMLChip8Emulator::v2()           const { return m_cpu.reg_V[0x2]; }
int QMLChip8Emulator::v3()           const { return m_cpu.reg_V[0x3]; }
int QMLChip8Emulator::v4()           const { return m_cpu.reg_V[0x4]; }
int QMLChip8Emulator::v5()           const { return m_cpu.reg_V[0x5]; }
int QMLChip8Emulator::v6()           const { return m_cpu.reg_V[0x6]; }
int QMLChip8Emulator::v7()           const { return m_cpu.reg_V[0x7]; }
int QMLChip8Emulator::v8()           const { return m_cpu.reg_V[0x8]; }
int QMLChip8Emulator::v9()           const { return m_cpu.reg_V[0x9]; }
int QMLChip8Emulator::vA()           const { return m_cpu.reg_V[0xA]; }
int QMLChip8Emulator::vB()           const { return m_cpu.reg_V[0xB]; }
int QMLChip8Emulator::vC()           const { return m_cpu.reg_V[0xC]; }
int QMLChip8Emulator::vD()           const { return m_cpu.reg_V[0xD]; }
int QMLChip8Emulator::vE()           const { return m_cpu.reg_V[0xE]; }
int QMLChip8Emulator::vF()           const { return m_cpu.reg_V[0xF]; }

int QMLChip8Emulator::stack0()       const { return m_cpu.stackAddress[0x0]; }
int QMLChip8Emulator::stack1()       const { return m_cpu.stackAddress[0x1]; }
int QMLChip8Emulator::stack2()       const { return m_cpu.stackAddress[0x2]; }
int QMLChip8Emulator::stack3()       const { return m_cpu.stackAddress[0x3]; }
int QMLChip8Emulator::stack4()       const { return m_cpu.stackAddress[0x4]; }
int QMLChip8Emulator::stack5()       const { return m_cpu.stackAddress[0x5]; }
int QMLChip8Emulator::stack6()       const { return m_cpu.stackAddress[0x6]; }
int QMLChip8Emulator::stack7()       const { return m_cpu.stackAddress[0x7]; }
int QMLChip8Emulator::stack8()       const { return m_cpu.stackAddress[0x8]; }
int QMLChip8Emulator::stack9()       const { return m_cpu.stackAddress[0x9]; }
int QMLChip8Emulator::stackA()       const { return m_cpu.stackAddress[0xA]; }
int QMLChip8Emulator::stackB()       const { return m_cpu.stackAddress[0xB]; }
int QMLChip8Emulator::stackC()       const { return m_cpu.stackAddress[0xC]; }
int QMLChip8Emulator::stackD()       const { return m_cpu.stackAddress[0xD]; }
int QMLChip8Emulator::stackE()       const { return m_cpu.stackAddress[0xE]; }
int QMLChip8Emulator::stackF()       const { return m_cpu.stackAddress[0xF]; }


void QMLChip8Emulator::setPC(int new_value)
{
    if (m_cpu.pc != new_value)
    {
	m_cpu.pc = new_value;
	emit pcChanged();
    }
}
void QMLChip8Emulator::setI(int new_value) { if (m_cpu.reg_I != new_value) { m_cpu.reg_I = new_value; emit iChanged(); } }

void QMLChip8Emulator::setSyncCounter(int new_value)
{
    if (m_cpu.synchronisationCounter != new_value)
    {
	m_cpu.synchronisationCounter = new_value;
	emit syncCounterChanged();
    }
}

void QMLChip8Emulator::setSoundCounter(int new_value)
{
    if (m_cpu.soundCounter != new_value) {
	m_cpu.soundCounter = new_value;
	emit soundCounterChanged();
    }
}

void QMLChip8Emulator::setStackPointer(int new_value)
{
    if (m_cpu.stackCount != new_value) {
	m_cpu.stackCount = new_value;
	emit stackPointerChanged();
    }
}

void QMLChip8Emulator::setV0(int new_value) { if (m_cpu.reg_V[0x0] != new_value) {  m_cpu.reg_V[0x0] = new_value; emit v0Changed(); } }
void QMLChip8Emulator::setV1(int new_value) { if (m_cpu.reg_V[0x1] != new_value) {  m_cpu.reg_V[0x1] = new_value; emit v1Changed(); } }
void QMLChip8Emulator::setV2(int new_value) { if (m_cpu.reg_V[0x2] != new_value) {  m_cpu.reg_V[0x2] = new_value; emit v2Changed(); } }
void QMLChip8Emulator::setV3(int new_value) { if (m_cpu.reg_V[0x3] != new_value) {  m_cpu.reg_V[0x3] = new_value; emit v3Changed(); } }
void QMLChip8Emulator::setV4(int new_value) { if (m_cpu.reg_V[0x4] != new_value) {  m_cpu.reg_V[0x4] = new_value; emit v4Changed(); } }
void QMLChip8Emulator::setV5(int new_value) { if (m_cpu.reg_V[0x5] != new_value) {  m_cpu.reg_V[0x5] = new_value; emit v5Changed(); } }
void QMLChip8Emulator::setV6(int new_value) { if (m_cpu.reg_V[0x6] != new_value) {  m_cpu.reg_V[0x6] = new_value; emit v6Changed(); } }
void QMLChip8Emulator::setV7(int new_value) { if (m_cpu.reg_V[0x7] != new_value) {  m_cpu.reg_V[0x7] = new_value; emit v7Changed(); } }
void QMLChip8Emulator::setV8(int new_value) { if (m_cpu.reg_V[0x8] != new_value) {  m_cpu.reg_V[0x8] = new_value; emit v8Changed(); } }
void QMLChip8Emulator::setV9(int new_value) { if (m_cpu.reg_V[0x9] != new_value) {  m_cpu.reg_V[0x9] = new_value; emit v9Changed(); } }
void QMLChip8Emulator::setVA(int new_value) { if (m_cpu.reg_V[0xA] != new_value) {  m_cpu.reg_V[0xA] = new_value; emit vAChanged(); } }
void QMLChip8Emulator::setVB(int new_value) { if (m_cpu.reg_V[0xB] != new_value) {  m_cpu.reg_V[0xB] = new_value; emit vBChanged(); } }
void QMLChip8Emulator::setVC(int new_value) { if (m_cpu.reg_V[0xC] != new_value) {  m_cpu.reg_V[0xC] = new_value; emit vCChanged(); } }
void QMLChip8Emulator::setVD(int new_value) { if (m_cpu.reg_V[0xD] != new_value) {  m_cpu.reg_V[0xD] = new_value; emit vDChanged(); } }
void QMLChip8Emulator::setVE(int new_value) { if (m_cpu.reg_V[0xE] != new_value) {  m_cpu.reg_V[0xE] = new_value; emit vEChanged(); } }
void QMLChip8Emulator::setVF(int new_value) { if (m_cpu.reg_V[0xF] != new_value) {  m_cpu.reg_V[0xF] = new_value; emit vFChanged(); } }

void QMLChip8Emulator::setStack0(int new_value) { if (m_cpu.stackAddress[0x0] != new_value) {  m_cpu.stackAddress[0x0] = new_value; emit stack0Changed(); } }
void QMLChip8Emulator::setStack1(int new_value) { if (m_cpu.stackAddress[0x1] != new_value) {  m_cpu.stackAddress[0x1] = new_value; emit stack1Changed(); } }
void QMLChip8Emulator::setStack2(int new_value) { if (m_cpu.stackAddress[0x2] != new_value) {  m_cpu.stackAddress[0x2] = new_value; emit stack2Changed(); } }
void QMLChip8Emulator::setStack3(int new_value) { if (m_cpu.stackAddress[0x3] != new_value) {  m_cpu.stackAddress[0x3] = new_value; emit stack3Changed(); } }
void QMLChip8Emulator::setStack4(int new_value) { if (m_cpu.stackAddress[0x4] != new_value) {  m_cpu.stackAddress[0x4] = new_value; emit stack4Changed(); } }
void QMLChip8Emulator::setStack5(int new_value) { if (m_cpu.stackAddress[0x5] != new_value) {  m_cpu.stackAddress[0x5] = new_value; emit stack5Changed(); } }
void QMLChip8Emulator::setStack6(int new_value) { if (m_cpu.stackAddress[0x6] != new_value) {  m_cpu.stackAddress[0x6] = new_value; emit stack6Changed(); } }
void QMLChip8Emulator::setStack7(int new_value) { if (m_cpu.stackAddress[0x7] != new_value) {  m_cpu.stackAddress[0x7] = new_value; emit stack7Changed(); } }
void QMLChip8Emulator::setStack8(int new_value) { if (m_cpu.stackAddress[0x8] != new_value) {  m_cpu.stackAddress[0x8] = new_value; emit stack8Changed(); } }
void QMLChip8Emulator::setStack9(int new_value) { if (m_cpu.stackAddress[0x9] != new_value) {  m_cpu.stackAddress[0x9] = new_value; emit stack9Changed(); } }
void QMLChip8Emulator::setStackA(int new_value) { if (m_cpu.stackAddress[0xA] != new_value) {  m_cpu.stackAddress[0xA] = new_value; emit stackAChanged(); } }
void QMLChip8Emulator::setStackB(int new_value) { if (m_cpu.stackAddress[0xB] != new_value) {  m_cpu.stackAddress[0xB] = new_value; emit stackBChanged(); } }
void QMLChip8Emulator::setStackC(int new_value) { if (m_cpu.stackAddress[0xC] != new_value) {  m_cpu.stackAddress[0xC] = new_value; emit stackCChanged(); } }
void QMLChip8Emulator::setStackD(int new_value) { if (m_cpu.stackAddress[0xD] != new_value) {  m_cpu.stackAddress[0xD] = new_value; emit stackDChanged(); } }
void QMLChip8Emulator::setStackE(int new_value) { if (m_cpu.stackAddress[0xE] != new_value) {  m_cpu.stackAddress[0xE] = new_value; emit stackEChanged(); } }
void QMLChip8Emulator::setStackF(int new_value) { if (m_cpu.stackAddress[0xF] != new_value) {  m_cpu.stackAddress[0xF] = new_value; emit stackFChanged(); } }








