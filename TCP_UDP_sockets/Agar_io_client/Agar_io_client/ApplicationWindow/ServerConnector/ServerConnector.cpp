#include "ServerConnector.h"

ServerConnector::ServerConnector(const QString& hostName, quint16 portNumber, QObject *parent)
    :QObject{ parent },
     m_socket{ new QTcpSocket(this) },
     m_nextDataBlockSize{ 0 }
    {
        m_socket->setParent(this);
        m_socket->connectToHost(hostName, portNumber);

        QObject::connect(m_socket, &QTcpSocket::readyRead,
                         this, &ServerConnector::slotReadyRead, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                         this, SLOT(slotSocketError(QAbstractSocket::SocketError)), Qt::ConnectionType::UniqueConnection);


    }


void ServerConnector::slotReadyRead()
{
    QDataStream inStream(m_socket);

    for (;;)
    {
        if(!m_nextDataBlockSize)
        {
            if(m_socket->bytesAvailable() < sizeof(qint16))
                break;

            inStream >> m_nextDataBlockSize;
        }

        if(m_socket->bytesAvailable() < m_nextDataBlockSize)
                break;

        ServerSignals typeMessage;
        inStream >> typeMessage;
        switch (typeMessage)
        {
            case ServerSignals::SetID           :
            {
                inStream >> m_ID;
                break;
            }
            case ServerSignals::EnterToArena    :
            {
                emit this->signalEnterToArena(this->m_ID);
                break;
            }
            case ServerSignals::LeaveArena      :
            {
            emit this->signalLeaveArena();
            break;
            }
        }
        m_nextDataBlockSize = 0;
    }
}
void ServerConnector::slotSocketError(QAbstractSocket::SocketError error)
{
    qDebug() << QString("Socket error: %1").arg(error);
}



void ServerConnector::slotSendPlayerWantPlay()
{    
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);
    outStream << quint16(0) << ServerConnector::UserSignals::UserWantPlay;

    outStream.device()->seek(0);
    outStream << quint16(dataBlock.size() - sizeof(quint16));
    m_socket->write(dataBlock);
}
void ServerConnector::slotSendPlayerWantExit()
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);
    outStream << quint16(0) << ServerConnector::UserSignals::UserWantExit;

    outStream.device()->seek(0);
    outStream << quint16(dataBlock.size() - sizeof(quint16));
    m_socket->write(dataBlock);
}
void ServerConnector::slotSendPlayerGoToThisPoint(QPoint targetPoint)
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);
    outStream << quint16(0)
              << ServerConnector::UserSignals::UserMoveToPoint
              << targetPoint;

    outStream.device()->seek(0);
    outStream << quint16(dataBlock.size() - sizeof(quint16));
    m_socket->write(dataBlock);
}
