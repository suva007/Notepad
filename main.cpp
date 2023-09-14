#include "mainwindow.h"

#include "customstyle.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("./Resources/Resources.qrc/imgs/icons/Copy.bmp"));
    QApplication::setStyle(new CustomStyle);
    MainWindow w;
//    QTabWidget* tabWidget = new QTabWidget;
//    w.setCentralWidget(tabWidget);
    w.setWindowTitle("Notepad");
    w.show();
    return a.exec();
}
