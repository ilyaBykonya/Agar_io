#include "ConnecterController.h"


ConnecterController::ConnecterController(quint16 portNumber, QWidget *parent)
    :QTextEdit{ parent },
     m_server{ new QTcpServer(this) },
     m_currentUserID{ 0 }
    {
        QObject::connect(m_server, &QTcpServer::newConnection,
                         this, &ConnecterController::slotNewConnection, Qt::ConnectionType::UniqueConnection);
        if(!m_server->listen(QHostAddress::SpecialAddress::Any, portNumber))
        {
            QMessageBox::critical(this, "Server error", m_server->errorString());
            return;
        }
        this->setReadOnly(true);


        //предварительно создаём арену, чтобы первый зашедший игрок
        //не ждал заполнения пустого поля
        m_arena = new Arena(this);
    }

void ConnecterController::slotNewConnection()
{
    User* newUser = new User(m_currentUserID, m_server->nextPendingConnection(), this);

    QObject::connect(newUser, &User::signalUserWantPlay,
                     this, &ConnecterController::slotUserWantPlay, Qt::ConnectionType::UniqueConnection);
    QObject::connect(newUser, &User::signalUserDisconnected,
                     this, &ConnecterController::slotUserDisconnected, Qt::ConnectionType::UniqueConnection);

    this->append(QString("Connect new user [%1][%2]").arg(m_currentUserID).arg(newUser->userIPAddress().toString()));
    ++m_currentUserID;
}
void ConnecterController::slotUserWantPlay(User* user)
{
    this->append(QString("User want play[%1]").arg(user->id()));
    m_arena->slotConnectUser(user);
}
void ConnecterController::slotUserDisconnected()
{
    User* user = qobject_cast<User*>(this->sender());
    if(user)
    {
        this->append(QString("User disconnected [%1]").arg(user->id()));
    }
}



