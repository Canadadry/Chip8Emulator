#include "qmlchip8emulator.h"
#include <QPainter>
#include <QFileDialog>
#include <QDebug>

QMLChip8Emulator::QMLChip8Emulator(QQuickPaintedItem *parent)
    : QQuickPaintedItem(parent)
    , m_screen(QSize(Chip8::Screen::SCREEN_WIDTH,Chip8::Screen::SCREEN_HEIGHT),QImage::Format_RGB32)
    , m_cpu(this)
{
    m_screen.fill(QColor::fromRgb(0,0,0));
}

void QMLChip8Emulator::paint(QPainter * painter)
{
    qDebug() << "painting to "<< QRect(0,0,width(),height());
    painter->drawImage(QRect(0,0,width(),height()),m_screen,QRect(0,0,Chip8::Screen::SCREEN_WIDTH,Chip8::Screen::SCREEN_HEIGHT));
}

void  QMLChip8Emulator::clearScreen()
{
    m_screen.fill(0);
    update();
}

void  QMLChip8Emulator::setPixel(int x, int y, Pixel p)
{
    m_screen.setPixel(x,y,p==Chip8::Screen::WHITE_PIXEL?qRgb(255,255,255):qRgb(0,0,0));
    update();
}

Chip8::Screen::Pixel QMLChip8Emulator::getPixel(int x, int y) const
{
    return m_screen.pixel(x,y) != qRgb(255,255,255) ? Chip8::Screen::WHITE_PIXEL : Chip8::Screen::BLACK_PIXEL;
}

void QMLChip8Emulator::step()
{
    m_cpu.step();
}

void QMLChip8Emulator::loadROM()
{
    QString fileName = QFileDialog::getOpenFileName(0,"Open Rom");
    FILE* fd = fopen(fileName.toStdString().c_str(),"r");

    qDebug() << "trying to open " << fileName;
    if(fd != NULL)
    {
	qDebug() << "opening ";

	char data[Chip8::CPU::MEMORY_SIZE];
	int size = fread(data,1,Chip8::CPU::MEMORY_SIZE,fd);
	qDebug() << "read " << size;
	if(size>0)
	{
	    m_cpu.loadROM((const unsigned char*)data,size);
	}
    }

}
