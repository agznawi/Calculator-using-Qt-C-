#include "interface.h"
#include "eqcalc.h"

#include <QApplication>

Interface *w;
EqCalc ec;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    w = new Interface;
    w->show();

    return a.exec();
}
