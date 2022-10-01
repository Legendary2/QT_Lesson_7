#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QStyleFactory>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QTranslator translator;
//  translator.load(QApplication::applicationName() + "_" + QLocale::system().name());
    a.installTranslator(&translator);

    qApp->setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.setWindowTitle("RK Editor");
    w.show();

    return a.exec();
}
