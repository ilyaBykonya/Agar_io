#include "ConnecterController/ConnecterController.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    srand(time(0));
    quint16 portNumber = 5555;


//    QFile readConnectToHostInfoFile("port_number.txt");
//    QTextStream readConnectToHostInfoStream(&readConnectToHostInfoFile);
//    if(!readConnectToHostInfoFile.exists())
//    {
//        QMessageBox::critical(nullptr, "Error", "Файл port_number.txt не найден.");
//        return -1;
//    }
//    if(!readConnectToHostInfoFile.open(QFile::OpenModeFlag::ReadOnly))
//    {
//        QMessageBox::critical(nullptr, "Error", "Файл port_number.txt не может быть открыт.");
//        return -1;
//    }
//    readConnectToHostInfoStream >> portNumber;


    ConnecterController wid(portNumber);
    wid.show();
    return app.exec();
}
