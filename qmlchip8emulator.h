#ifndef QMLCHIP8EMULATOR_H
#define QMLCHIP8EMULATOR_H

#include <QQuickPaintedItem>
#include <QImage>
#include <chip8/chip8.h>

class QMLChip8Emulator : public QQuickPaintedItem, public Chip8::Screen
{
    Q_OBJECT
public:
    explicit QMLChip8Emulator(QQuickPaintedItem *parent = 0);

    virtual void	paint(QPainter * painter) ;

    virtual void  clearScreen();
    virtual void  setPixel(int x, int y, Pixel p);
    virtual Pixel getPixel(int x, int y) const;

    Q_INVOKABLE void step();
    Q_INVOKABLE void loadROM();

private:
    QImage m_screen;
    Chip8::CPU m_cpu;
};

#endif // QMLCHIP8EMULATOR_H
