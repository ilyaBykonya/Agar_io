#include "ArenaWidget.h"


ArenaWidget::ArenaWidget(quint32 thisPlayerID, QWidget *parent)
    :QWidget{ parent },
     m_trapPicture{ "D:/bufFiles/tcp_udp__sockets_meet_up/agar_io_custom/Client/Agar_io_client/src/pic/trap.png" },
     m_UDPSocket{ new QUdpSocket(this) },
     m_thisPlayerID{ thisPlayerID }
    {
        this->setFixedSize(arenaSize, arenaSize);
        QObject::connect(m_UDPSocket, &QUdpSocket::readyRead,
                        this, &ArenaWidget::slotUpdateArena, Qt::ConnectionType::UniqueConnection);
        if(!m_UDPSocket->bind(QHostAddress::SpecialAddress::LocalHost, 52000))
        {
            QMessageBox::critical(this, "UDP socket error", m_UDPSocket->errorString());
            qApp->quit();
        }
    }


void ArenaWidget::slotUpdateArena()
{
    m_allPlayersList.clear();
    m_foodList.clear();
    m_trapList.clear();

    QByteArray dataBlock;
    do
    {
        dataBlock.resize(m_UDPSocket->pendingDatagramSize());
        m_UDPSocket->readDatagram(dataBlock.data(), dataBlock.size());
    } while(m_UDPSocket->hasPendingDatagrams());

    QDataStream inStream(&dataBlock, QIODevice::OpenModeFlag::ReadOnly);
    inStream.setVersion(QDataStream::Version::Qt_DefaultCompiledVersion);

    quint16 amountOfPlayers;
    inStream >> amountOfPlayers;
    for(quint16 i = 0; i < amountOfPlayers; ++i)
    {
        quint32 userID;
        quint32 weight;
        QPoint position;
        QColor color;

        inStream >> userID >> weight >>  position >> color;
        m_allPlayersList.push_back(Player{ weight, position, color });

        if(userID == m_thisPlayerID)
            emit this->signalUpdateInfo((position + QPoint(weight / 10, weight / 10)), weight);
    }

    quint16 amountOfFood;
    inStream >> amountOfFood;
    for(quint16 i = 0; i < amountOfFood; ++i)
    {
        QPoint food;
        inStream >> food;
        m_foodList << food;
    }

    quint16 amountOfTraps;
    inStream >> amountOfTraps;
    for(quint16 i = 0; i < amountOfTraps; ++i)
    {
        QPoint trap;
        inStream >> trap;
        m_trapList << trap;
    }

    this->update();
}
void ArenaWidget::mousePressEvent(QMouseEvent *event)
{
    emit this->signalPlayerGoToThisPoint(event->pos());
}
void ArenaWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(0, 0, 0), 3));
    painter.setBrush(Qt::BrushStyle::NoBrush);
    for(auto it = m_foodList.begin(); it != m_foodList.end(); ++it)
        painter.drawPoint(*it);

    for(auto it = m_trapList.begin(); it != m_trapList.end(); ++it)
        painter.drawPixmap(QRect(it->x() - 15, it->y() - 15, 30, 30), m_trapPicture);

    painter.setPen(QPen(QColor(0, 0, 0), 1));
    for(auto it = m_allPlayersList.begin(); it != m_allPlayersList.end(); ++it)
    {
        painter.setBrush(QBrush(it->m_playerColor));

        painter.drawEllipse(QRect(it->m_currentPosition,
                                  QSize(it->m_weight / weightToDiameterRatio + diametrAllowance,
                                        it->m_weight / weightToDiameterRatio + diametrAllowance)));
    }
}
