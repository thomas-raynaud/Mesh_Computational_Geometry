#include "ui/MainWindow.h"
#include <QApplication>


int main(int argc, char *argv[]) {
    srand(time(NULL));
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}