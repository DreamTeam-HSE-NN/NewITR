#include "NewITR.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NewITR w;
    w.show();
    return a.exec();
}
