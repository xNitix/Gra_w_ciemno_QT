#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString windowName = QString::fromStdString("Gra w ciemno");
    w.setWindowTitle(windowName);
    w.show();
    return a.exec();
}
