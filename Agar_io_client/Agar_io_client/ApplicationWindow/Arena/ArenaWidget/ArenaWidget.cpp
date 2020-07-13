#include "ArenaWidget.h"


ArenaWidget::ArenaWidget(QWidget *parent)
    :QWidget{ parent },
     m_trapPicture{ "../src/pic/trap.png" }
    {
        this->setFixedSize(arenaSize, arenaSize);
    }

void ArenaWidget::mousePressEvent(QMouseEvent *event)
{
    emit this->signalPlayerGoToThisPoint(event->pos());
}

void ArenaWidget::slotUpdateArena(quint32 thisPlayerID, QDataStream& inStream)
{
    m_allPlayersList.clear();
    m_foodList.clear();
    m_trapList.clear();

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

        if(userID == thisPlayerID)
            emit this->signalUpdateInfo(thisPlayerID, (position + QPoint(weight / 10, weight / 10)), weight);
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
