#include "ConnecterController.h"


ConnecterController::ConnecterController(quint16 portNumber, QWidget *parent)
    :QTextEdit{ parent },
     m_server{ new QWebSocketServer("my_agar_io_server", QWebSocketServer::SslMode::NonSecureMode, this) },
     m_currentUserID{ 0 },
     m_currentArenaID{ 0 }
    {
        if(!m_server->listen(QHostAddress::SpecialAddress::Any, portNumber))
        {
            QMessageBox::critical(this, "Server error", m_server->errorString());
            return;
        }

        this->setReadOnly(true);
        QObject::connect(m_server, &QWebSocketServer::newConnection,
                         this, &ConnecterController::slotNewConnection, Qt::ConnectionType::UniqueConnection);
    }

void ConnecterController::slotNewConnection()
{
    this->append(QString("Connect new user [%1]").arg(m_currentUserID));
    User* newUser = new User(m_currentUserID, m_server->nextPendingConnection(), this);
    QObject::connect(newUser, &User::signalUserWantPlay,
                     this, &ConnecterController::slotUserWantPlay, Qt::ConnectionType::UniqueConnection);
    ++m_currentUserID;
}

void ConnecterController::slotUserWantPlay(User* user)
{
    this->append(QString("User want play[%1]").arg(user->id()));
    for(auto it = m_instancesList.begin(); it != m_instancesList.end(); ++it)
    {
        if((*it)->amountOfPlayersInInstance() < 100)
        {
            (*it)->slotConnectUser(user);
            return;
        }
    }

    Arena* newArena = new Arena(m_currentArenaID, this);
    m_instancesList.push_back(newArena);
    newArena->slotConnectUser(user);
    ++m_currentArenaID;
}
