#include "Player.h"

Player::Player(User* user, QObject *parent)
    :QObject{ parent },
     m_playerColor(rand() % 256, rand() % 256, rand() % 256),
     m_user{ user },
     m_weight{ 100 },
     m_position{ 50, 50 },
     m_playerSpeed{ 0, 0 },
     m_growUpCounter{ 0 }
    {
        QObject::connect(m_user, &User::signalUserGoToThisPoint,
                         this, &Player::slotUserGoToThisPoint, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_user, &User::signalUserWantExit,
                         this, &Player::slotUserExitFromGame, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_user, &User::signalUserDisconnected,
                         this, &Player::singalUserDisconnected, Qt::ConnectionType::UniqueConnection);

        QObject::connect(this, &Player::signalUpdateArena,
                         m_user, &User::slotSendUpdateGameInstance, Qt::ConnectionType::UniqueConnection);
        QObject::connect(this, &Player::signalExitFromArena,
                         m_user, &User::slotExitFromArena, Qt::ConnectionType::UniqueConnection);
    }


quint32 Player::weight() const
{
    return m_weight;
}
quint32 Player::id() const
{
    return m_user->id();
}

QPointF Player::currentPosition() const
{
    return m_position;
}
QPointF Player::centerThisUserPoint() const
{
    return this->m_position + QPointF(this->m_weight / weightToRadiusRatio + radiusAllowance,
                   this->m_weight / weightToRadiusRatio + radiusAllowance);
}


bool Player::containsInRound(QPoint p)
{
    QPointF center = this->centerThisUserPoint();
    return (pow(p.x() - center.x(), 2) + pow(p.y() - center.y(), 2)) < (pow(this->m_weight / 20 + radiusAllowance, 2));
}
void Player::doStep()
{
    m_position += QPointF(m_playerSpeed.width(), m_playerSpeed.height());

    if(m_position.x() < 0)
        m_position.setX(1);
    else if(m_position.x() > (arenaSize - this->weight() / weightToDiameterRatio - diametrAllowance))
        m_position.setX(arenaSize - this->weight() / weightToDiameterRatio - diametrAllowance - 1);

    if(m_position.y() < 0)
        m_position.setY(1);
    else if(m_position.y() > (arenaSize - this->weight() / weightToDiameterRatio - diametrAllowance))
        m_position.setY(arenaSize - this->weight() / weightToDiameterRatio - diametrAllowance - 1);
}

void Player::growUp(quint32 w)
{
    m_weight += w;
    ++m_growUpCounter;
}
void Player::decrease(quint32 w)
{
    if(m_weight < w)
    {
        m_weight = 0;
    }
    else
    {
        m_weight -= w;
    }
}


QDataStream& operator<<(QDataStream& out, const Player* player)
{
    out << player->id()
        << player->m_weight
        << player->m_position.toPoint()
        << player->m_playerColor;
    return out;
}

void Player::slotUserGoToThisPoint(QPoint p)
{
    QPointF center = this->centerThisUserPoint();
    qreal distance = sqrt(qreal(pow((p.x() - center.x()), 2) + pow((p.y() - center.y()), 2)));
    qreal amountOfSteps = distance * 25 / playerLinearSpeed;
    m_playerSpeed.setWidth(qreal(p.x() - center.x()) / amountOfSteps);
    m_playerSpeed.setHeight(qreal(p.y() - center.y()) / amountOfSteps);
}
void Player::slotUserExitFromGame()
{
    m_weight = 0;
}
