#include "Player.h"

Player::Player(QPoint startPosition, User* user, QObject *parent)
    :QObject{ parent },
     m_playerColor(rand() % 256, rand() % 256, rand() % 256),
     m_position{ startPosition },
     m_playerSpeed{ 0, 0 },
     m_user{ user },
     m_weight{ startPlayerWeight },
     m_arenaSocket{ new QUdpSocket(this) }
    {
        QObject::connect(m_user, &User::signalUserGoToThisPoint,
                         this, &Player::slotUserGoToThisPoint, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_user, &User::signalUserWantExit,
                         this, &Player::slotUserExitFromGame, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_user, &User::signalUserDisconnected,
                         this, &Player::slotUserExitFromGame, Qt::ConnectionType::UniqueConnection);


        m_arenaSocket->connectToHost(m_user->userIPAddress(), 52000 + this->id());
    }

Player::~Player()
{
    m_user->slotLeaveArena();
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
    qreal amountOfSteps = distance * serverTPS / playerLinearSpeed;
    m_playerSpeed.setWidth(qreal(p.x() - center.x()) / amountOfSteps);
    m_playerSpeed.setHeight(qreal(p.y() - center.y()) / amountOfSteps);
}
void Player::slotUserExitFromGame()
{
    //Если какой-то игрок захотел выйти,
    //то мы просто сбрасываем на 0 его массу
    //и при следующем обновлении арена его удалит
    m_weight = 0;
}


void Player::slotUpdateArena(const QByteArray& dataBlock)
{
    m_arenaSocket->write(dataBlock);
}


