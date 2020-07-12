#include "User.h"

User::User(quint32 id, QWebSocket* socket, QObject *parent)
    :QObject{ parent },
     m_socket{ socket },
     m_ID{ id },
     m_state{ User::UserState::UserInLobbi }
    {
        m_socket->setParent(this);
        QObject::connect(m_socket, &QWebSocket::binaryMessageReceived,
                         this, &User::slotReadyRead, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_socket, &QWebSocket::disconnected,
                         this, &QObject::deleteLater, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_socket, &QWebSocket::disconnected,
                         this, &User::signalUserDisconnected, Qt::ConnectionType::UniqueConnection);


        QByteArray dataBlock;
        QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);
        outStream << User::ServerSignals::SetID
                  << m_ID;

        m_socket->sendBinaryMessage(dataBlock);
    }

quint32 User::id() const { return m_ID; }

void User::slotReadyRead(const QByteArray& dataBlock)
{
    QDataStream inStream(dataBlock);

    User::UserActions typeMessage;
    inStream >> typeMessage;
    switch (typeMessage)
    {
        case User::UserActions::UserWantPlay    :
        {
            m_state = User::UserState::UserInGame;
            emit this->signalUserWantPlay(this);
            break;
        }
        case User::UserActions::UserMoveToPoint :
        {
            QPoint p;
            inStream >> p;
            emit this->signalUserGoToThisPoint(p);
            break;
        }
        case User::UserActions::UserWantExit    :
        {
            emit this->signalUserWantExit();
            break;
        }
    }
}
void User::slotSendUpdateGameInstance(const QByteArray& dataBlock)
{
    if(m_state == User::UserState::UserInLobbi)
        return;

    m_socket->sendBinaryMessage(dataBlock);
}
void User::slotExitFromArena()
{
    if(m_state == User::UserState::UserInLobbi)
        return;

    m_state = User::UserState::UserInLobbi;
    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);

    outStream << User::ServerSignals::LeaveArena;
    m_socket->sendBinaryMessage(dataBlock);
}
