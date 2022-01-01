#include "ui/MainWindow.h"
#include <QApplication>


int main(int argc, char *argv[]) {
    std::setlocale(LC_NUMERIC, "C");
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}