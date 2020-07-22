#include "User.h"

User::User(quint32 id, QTcpSocket* socket, QObject *parent)
    :QObject{ parent },
     m_nextDataBlockSize{ 0 },
     m_socket{ socket },
     m_ID{ id }
    {


        m_socket->setParent(this);
        QObject::connect(m_socket, &QTcpSocket::readyRead,
                         this, &User::slotReadyRead, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_socket, &QTcpSocket::disconnected,
                         this, &QObject::deleteLater, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_socket, &QTcpSocket::disconnected,
                         this, &User::signalUserDisconnected, Qt::ConnectionType::UniqueConnection);


        QByteArray dataBlock;
        QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

        outStream << quint16(0) << User::ServerSignals::SetID << m_ID;
        outStream.device()->seek(0);
        outStream << quint16(dataBlock.size() - sizeof(quint16));
        m_socket->write(dataBlock);
    }

QHostAddress User::userIPAddress() const
{
    return m_socket->peerAddress();
}
quint32 User::id() const { return m_ID; }

void User::slotReadyRead()
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

        User::UserSignals typeMessage;
        inStream >> typeMessage;
        switch (typeMessage)
        {
            case User::UserSignals::UserWantPlay    :
            {
                emit this->signalUserWantPlay(this);
                break;
            }
            case User::UserSignals::UserMoveToPoint :
            {
                QPoint p;
                inStream >> p;
                emit this->signalUserGoToThisPoint(p);
                break;
            }
            case User::UserSignals::UserWantExit    :
            {
                emit this->signalUserWantExit();
                break;
            }
        }
        m_nextDataBlockSize = 0;
    }
}
void User::slotEnterToArena()
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

    outStream << quint16(0) << User::ServerSignals::EnterToArena;
    outStream.device()->seek(0);
    outStream << quint16(dataBlock.size() - sizeof(quint16));
    m_socket->write(dataBlock);
}
void User::slotLeaveArena()
{
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

    outStream << quint16(0) << User::ServerSignals::LeaveArena;
    outStream.device()->seek(0);
    outStream << quint16(dataBlock.size() - sizeof(quint16));
    m_socket->write(dataBlock);
}

