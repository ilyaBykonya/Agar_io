#include "ServerConnector.h"

ServerConnector::ServerConnector(const QString& hostName, quint16 portNumber, QObject *parent)
    :QObject{ parent },
     m_webSocket{ new QWebSocket(QString("ws://%1:%2").arg(hostName).arg(portNumber), QWebSocketProtocol::Version::VersionLatest, this) }
    {
        QObject::connect(m_webSocket, &QWebSocket::binaryMessageReceived,
                         this, &ServerConnector::slotReadyRead, Qt::ConnectionType::UniqueConnection);


        m_webSocket->open(QUrl(QString("ws://%1:%2").arg(hostName).arg(portNumber)));
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
