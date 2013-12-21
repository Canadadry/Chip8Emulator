#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QQmlEngine>
#include "qmlchip8emulator.h"
#include <QApplication>
#include <cmath>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication app(argc, argv);

    qmlRegisterType<QMLChip8Emulator>("Chip8", 1, 0, "Emulator");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/Chip8Emulator/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
