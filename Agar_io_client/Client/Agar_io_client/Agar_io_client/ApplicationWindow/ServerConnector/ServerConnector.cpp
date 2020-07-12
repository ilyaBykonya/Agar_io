#include "ServerConnector.h"

ServerConnector::ServerConnector(const QUrl& hostAddress, QObject *parent)
    :QObject{ parent },
     m_webSocket{ new QWebSocket("wss://127.0.0.1", QWebSocketProtocol::Version::VersionLatest, this) }
    {
        m_webSocket->setParent(this);

        QObject::connect(m_webSocket, &QWebSocket::binaryMessageReceived,
                         this, &ServerConnector::slotReadyRead, Qt::ConnectionType::UniqueConnection);

        QObject::connect(m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                         this, SLOT(slotSocketError(QAbstractSocket::SocketError)), Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_webSocket, &QWebSocket::sslErrors,
                         this, &ServerConnector::slotSslErrors, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_webSocket, &QWebSocket::connected,
                         this, &ServerConnector::slotConnection, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_webSocket, &QWebSocket::disconnected,
                         this, &ServerConnector::slotSocketDisconnected, Qt::ConnectionType::UniqueConnection);

        m_webSocket->open(hostAddress);
    }


void ServerConnector::slotConnection()
{
    qDebug() << "Succesfully connect to host";
}
void ServerConnector::slotReadyRead(const QByteArray& dataBlock)
{
    QDataStream inStream(dataBlock);

    ServerSignals typeMessage;
    inStream >> typeMessage;
    switch (typeMessage)
    {
        case ServerSignals::SetID          :
        {
            inStream >> m_ID;
            break;
        }
        case ServerSignals::UpdateArena    :
        {
            emit this->signalUpdateArena(this->m_ID, dataBlock);
            break;
        }
        case ServerSignals::LeaveArena     :
        {
            emit this->signalExitFromArena();
            break;
        }
    }
}


void ServerConnector::slotSocketError(QAbstractSocket::SocketError error)
{
    qDebug() << QString("Socket error: %1").arg(error);
}
void ServerConnector::slotSslErrors(const QList<QSslError> &errors)
{
    m_webSocket->ignoreSslErrors();
    qDebug() << QString("Ssl errors:");
    for(auto i: errors)
    {
        qDebug() << QString("   Ssl error: %1").arg(i.errorString());
    }
}
void ServerConnector::slotSocketDisconnected()
{
    qDebug() << "Disconnect from host";
}
void ServerConnector::slotSocketStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << QString("New socket state: %1").arg(state);
}








void ServerConnector::slotSendPlayerWantPlay()
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

    outStream << ServerConnector::UserActions::UserWandPlay;
    m_webSocket->sendBinaryMessage(dataBlock);
}
void ServerConnector::slotSendPlayerGoToThisPoint(QPoint targetPoint)
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

    outStream << ServerConnector::UserActions::UserMoveToPoint
              << targetPoint;
    m_webSocket->sendBinaryMessage(dataBlock);
}
void ServerConnector::slotSendPlayerWantExit()
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

    outStream << ServerConnector::UserActions::UserWantExit;
    m_webSocket->sendBinaryMessage(dataBlock);
}
