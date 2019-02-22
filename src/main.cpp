#include <QApplication>

#include "application.h"

int main(int argc, char* argv[])
{
    QApplication qapp(argc, argv);

    Application app(argc, argv);

    return QApplication::exec();
}