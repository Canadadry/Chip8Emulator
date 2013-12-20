#ifndef QMLCHIP8EMULATOR_H
#define QMLCHIP8EMULATOR_H

#include <QQuickPaintedItem>
#include <QImage>
#include <chip8/chip8.h>

class QMLChip8Emulator : public QQuickPaintedItem, public Chip8::Screen
{
    Q_OBJECT
    Q_PROPERTY(int pc READ pc WRITE setPC NOTIFY pcChanged)
    Q_PROPERTY(int  i READ  i WRITE setI  NOTIFY  iChanged)
    Q_PROPERTY(int syncCounter READ syncCounter WRITE setSyncCounter NOTIFY syncCounterChanged)
    Q_PROPERTY(int soundCounter READ soundCounter WRITE setSoundCounter NOTIFY soundCounterChanged)
    Q_PROPERTY(int stackPointer READ stackPointer WRITE setStackPointer NOTIFY stackPointerChanged)
    Q_PROPERTY(int v0 READ v0 WRITE setV0 NOTIFY v0Changed)
    Q_PROPERTY(int v1 READ v1 WRITE setV1 NOTIFY v1Changed)
    Q_PROPERTY(int v2 READ v2 WRITE setV2 NOTIFY v2Changed)
    Q_PROPERTY(int v3 READ v3 WRITE setV3 NOTIFY v3Changed)
    Q_PROPERTY(int v4 READ v4 WRITE setV4 NOTIFY v4Changed)
    Q_PROPERTY(int v5 READ v5 WRITE setV5 NOTIFY v5Changed)
    Q_PROPERTY(int v6 READ v6 WRITE setV6 NOTIFY v6Changed)
    Q_PROPERTY(int v7 READ v7 WRITE setV7 NOTIFY v7Changed)
    Q_PROPERTY(int v8 READ v8 WRITE setV8 NOTIFY v8Changed)
    Q_PROPERTY(int v9 READ v9 WRITE setV9 NOTIFY v9Changed)
    Q_PROPERTY(int vA READ vA WRITE setVA NOTIFY vAChanged)
    Q_PROPERTY(int vB READ vB WRITE setVB NOTIFY vBChanged)
    Q_PROPERTY(int vC READ vC WRITE setVC NOTIFY vCChanged)
    Q_PROPERTY(int vD READ vD WRITE setVD NOTIFY vDChanged)
    Q_PROPERTY(int vE READ vE WRITE setVE NOTIFY vEChanged)
    Q_PROPERTY(int vF READ vF WRITE setVF NOTIFY vFChanged)
    Q_PROPERTY(int stack0 READ stack0 WRITE setStack0 NOTIFY stack0Changed)
    Q_PROPERTY(int stack1 READ stack1 WRITE setStack1 NOTIFY stack1Changed)
    Q_PROPERTY(int stack2 READ stack2 WRITE setStack2 NOTIFY stack2Changed)
    Q_PROPERTY(int stack3 READ stack3 WRITE setStack3 NOTIFY stack3Changed)
    Q_PROPERTY(int stack4 READ stack4 WRITE setStack4 NOTIFY stack4Changed)
    Q_PROPERTY(int stack5 READ stack5 WRITE setStack5 NOTIFY stack5Changed)
    Q_PROPERTY(int stack6 READ stack6 WRITE setStack6 NOTIFY stack6Changed)
    Q_PROPERTY(int stack7 READ stack7 WRITE setStack7 NOTIFY stack7Changed)
    Q_PROPERTY(int stack8 READ stack8 WRITE setStack8 NOTIFY stack8Changed)
    Q_PROPERTY(int stack9 READ stack9 WRITE setStack9 NOTIFY stack9Changed)
    Q_PROPERTY(int stackA READ stackA WRITE setStackA NOTIFY stackAChanged)
    Q_PROPERTY(int stackB READ stackB WRITE setStackB NOTIFY stackBChanged)
    Q_PROPERTY(int stackC READ stackC WRITE setStackC NOTIFY stackCChanged)
    Q_PROPERTY(int stackD READ stackD WRITE setStackD NOTIFY stackDChanged)
    Q_PROPERTY(int stackE READ stackE WRITE setStackE NOTIFY stackEChanged)
    Q_PROPERTY(int stackF READ stackF WRITE setStackF NOTIFY stackFChanged)

public:
    explicit QMLChip8Emulator(QQuickPaintedItem *parent = 0);

    virtual void	paint(QPainter * painter) ;

    virtual void  clearScreen();
    virtual void  setPixel(int x, int y, Pixel p);
    virtual Pixel getPixel(int x, int y) const;

    Q_INVOKABLE void step();
    Q_INVOKABLE void loadROM();
    Q_INVOKABLE int readMemoryAt(int address);
    Q_INVOKABLE void writeByteInMemoryAt(int byte, int address);

    int pc() const;
    int i() const;
    int syncCounter() const;
    int soundCounter() const;
    int stackPointer() const;
    int v0() const;
    int v1() const;
    int v2() const;
    int v3() const;
    int v4() const;
    int v5() const;
    int v6() const;
    int v7() const;
    int v8() const;
    int v9() const;
    int vA() const;
    int vB() const;
    int vC() const;
    int vD() const;
    int vE() const;
    int vF() const;

    int stack0() const;
    int stack1() const;
    int stack2() const;
    int stack3() const;
    int stack4() const;
    int stack5() const;
    int stack6() const;
    int stack7() const;
    int stack8() const;
    int stack9() const;
    int stackA() const;
    int stackB() const;
    int stackC() const;
    int stackD() const;
    int stackE() const;
    int stackF() const;

public slots:
    void setPC(int new_value);
    void setI(int new_value);
    void setSyncCounter(int new_value);
    void setSoundCounter(int new_value);
    void setStackPointer(int new_value);
    void setV0(int new_value);
    void setV1(int new_value);
    void setV2(int new_value);
    void setV3(int new_value);
    void setV4(int new_value);
    void setV5(int new_value);
    void setV6(int new_value);
    void setV7(int new_value);
    void setV8(int new_value);
    void setV9(int new_value);
    void setVA(int new_value);
    void setVB(int new_value);
    void setVC(int new_value);
    void setVD(int new_value);
    void setVE(int new_value);
    void setVF(int new_value);
    void setStack0(int new_value);
    void setStack1(int new_value);
    void setStack2(int new_value);
    void setStack3(int new_value);
    void setStack4(int new_value);
    void setStack5(int new_value);
    void setStack6(int new_value);
    void setStack7(int new_value);
    void setStack8(int new_value);
    void setStack9(int new_value);
    void setStackA(int new_value);
    void setStackB(int new_value);
    void setStackC(int new_value);
    void setStackD(int new_value);
    void setStackE(int new_value);
    void setStackF(int new_value);

signals:
    void pcChanged();
    void iChanged();
    void syncCounterChanged();
    void soundCounterChanged();
    void stackPointerChanged();
    void v0Changed();
    void v1Changed();
    void v2Changed();
    void v3Changed();
    void v4Changed();
    void v5Changed();
    void v6Changed();
    void v7Changed();
    void v8Changed();
    void v9Changed();
    void vAChanged();
    void vBChanged();
    void vCChanged();
    void vDChanged();
    void vEChanged();
    void vFChanged();
    void stack0Changed();
    void stack1Changed();
    void stack2Changed();
    void stack3Changed();
    void stack4Changed();
    void stack5Changed();
    void stack6Changed();
    void stack7Changed();
    void stack8Changed();
    void stack9Changed();
    void stackAChanged();
    void stackBChanged();
    void stackCChanged();
    void stackDChanged();
    void stackEChanged();
    void stackFChanged();

private:
    QImage m_screen;
    Chip8::CPU m_cpu;
};

#endif // QMLCHIP8EMULATOR_H
