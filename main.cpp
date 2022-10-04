#include "mainwindow.h"
#include <unordered_map>
#include <QApplication>

int main(int argc, char *argv[])
{
    // std::unordered_map<std::string, std::pair<double, double> > airports;
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(800, 650);
    w.setWindowTitle("Airports");
    w.show();
    return a.exec();
}
