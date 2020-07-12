#include "ConnecterController.h"


ConnecterController::ConnecterController(quint16 portNumber, QWidget *parent)
    :QTextEdit{ parent },
     m_server{ new QWebSocketServer("my_agar_io_server", QWebSocketServer::SslMode::SecureMode, this) },
     m_currentUserID{ 0 },
     m_currentArenaID{ 0 }
    {
    //ssl-verification
    //
        QSslConfiguration sslConfiguration;
        QFile certFile("D:/C++/agar_io_custom/Server/Agar_io_server/SslOptionsFiles/test_ssl_12_07.crt");
        QFile keyFile("D:/C++/agar_io_custom/Server/Agar_io_server/SslOptionsFiles/test_ssl_12_07.key");
        certFile.open(QIODevice::ReadOnly);
        keyFile.open(QIODevice::ReadOnly);
        QSslCertificate certificate(&certFile, QSsl::EncodingFormat::Pem);
        QSslKey sslKey(&keyFile, QSsl::KeyAlgorithm::Rsa, QSsl::EncodingFormat::Pem);
        certFile.close();
        keyFile.close();

        sslConfiguration.setPeerVerifyMode(QSslSocket::PeerVerifyMode::QueryPeer);
        sslConfiguration.setLocalCertificate(certificate);
        sslConfiguration.setPrivateKey(sslKey);
        m_server->setSslConfiguration(sslConfiguration);

        if(!m_server->listen(QHostAddress::SpecialAddress::Any, portNumber))
        {
            QMessageBox::critical(this, "Server error", m_server->errorString());
            return;
        }

        this->setReadOnly(true);


        QObject::connect(m_server, &QWebSocketServer::newConnection,
                         this, &ConnecterController::slotNewConnection, Qt::ConnectionType::UniqueConnection);




        Arena* newArena = new Arena(m_currentArenaID, this);
        m_instancesList.push_back(newArena);
        ++m_currentArenaID;
    }

void ConnecterController::slotNewConnection()
{
    this->append(QString("Connect new user [%1]").arg(m_currentUserID));
    User* newUser = new User(m_currentUserID, m_server->nextPendingConnection(), this);
    QObject::connect(newUser, &User::signalUserWantPlay,
                     this, &ConnecterController::slotUserWantPlay, Qt::ConnectionType::UniqueConnection);
    QObject::connect(newUser, &User::signalUserDisconnected,
                     this, &ConnecterController::slotUserDisconnected, Qt::ConnectionType::UniqueConnection);
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
void ConnecterController::slotUserDisconnected()
{
    User* user = qobject_cast<User*>(this->sender());
    if(user)
    {
        this->append(QString("User disconnected [%1]").arg(user->id()));
    }
}


void ConnecterController::slotPeerVerifyError(const QSslError& error)
{
    qDebug() << error.errorString();
}
