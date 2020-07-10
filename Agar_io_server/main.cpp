#include "ConnecterController/ConnecterController.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ConnecterController wid(5555);


    wid.show();
    return app.exec();
}
