#include <QApplication>
#include <QDebug>
#include "main_window.h"
#include "mode_chooser.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    ModeChooser modeChooser;
    if(modeChooser.exec() == QDialog::Rejected)
        return 0;

    UserInfo userInfo = modeChooser.getUserInfo();

    MainWindow mainWindow(userInfo);
    mainWindow.show();
    if(!mainWindow.setup())
        return 0;

    return QApplication::exec();
}