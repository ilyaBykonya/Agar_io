#include <QApplication>
#include "ApplicationWindow/ApplicationWindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ApplicationWindow wid("18.222.125.83", 5555);
    QObject::connect(&wid, &ApplicationWindow::signalExitButtonWasClicked,
                     qApp, &QApplication::quit);
    wid.show();
    return a.exec();
}
