#include "ServerConnector.h"

ServerConnector::ServerConnector(const QUrl& hostAddress, QObject *parent)
    :QObject{ parent },
     m_webSocket{ new QWebSocket("", QWebSocketProtocol::Version::VersionLatest, this) }
    {
        m_webSocket->setParent(this);

        QObject::connect(m_webSocket, &QWebSocket::binaryMessageReceived,
                         this, &ServerConnector::slotReadyRead, Qt::ConnectionType::UniqueConnection);

        QObject::connect(m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                         this, SLOT(slotSocketError(QAbstractSocket::SocketError)), Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_webSocket, &QWebSocket::sslErrors,
                         this, &ServerConnector::slotSslErrors, Qt::ConnectionType::UniqueConnection);

        m_webSocket->open(hostAddress);
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
            emit this->signalUpdateArena(this->m_ID, inStream);
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
    //я игнорирую ошибки ssl, т.к. на сервере у меня стоит
    //самоподписанный ssl-сертификат
    m_webSocket->ignoreSslErrors();
    qDebug() << QString("Ssl errors:");
    for(auto i: errors)
    {
        qDebug() << QString("   Ssl error: %1").arg(i.errorString());
    }
}




void ServerConnector::slotSendPlayerWantPlay()
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

    outStream << ServerConnector::UserSignals::UserWandPlay;
    m_webSocket->sendBinaryMessage(dataBlock);
}
void ServerConnector::slotSendPlayerGoToThisPoint(QPoint targetPoint)
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

    outStream << ServerConnector::UserSignals::UserMoveToPoint
              << targetPoint;
    m_webSocket->sendBinaryMessage(dataBlock);
}
void ServerConnector::slotSendPlayerWantExit()
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

    outStream << ServerConnector::UserSignals::UserWantExit;
    m_webSocket->sendBinaryMessage(dataBlock);
}
