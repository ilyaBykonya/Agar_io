#include "Arena.h"

Arena::Arena(quint32 arenaID, QObject *parent)
    :QObject{ parent },
     m_instanceID{ arenaID }
    {
        this->startTimer(40, Qt::TimerType::PreciseTimer);
    }

quint32 Arena::amountOfPlayersInInstance() const
{
    return m_playersList.size();
}
quint32 Arena::arenaID() const
{
    return m_instanceID;
}

void Arena::slotConnectUser(User* user)
{
    Player* newPlayer = new Player(user, this);
    m_playersList.push_back(newPlayer);

    QObject::connect(this, &Arena::signalUpdateArena,
                     newPlayer, &Player::signalUpdateArena, Qt::ConnectionType::UniqueConnection);

    QObject::connect(newPlayer, &Player::singalUserDisconnected,
                     this, &Arena::slotDisconnectUser, Qt::ConnectionType::UniqueConnection);
}
void Arena::slotDisconnectUser()
{
    Player* player = qobject_cast<Player*>(this->sender());
    if(player)
    {
        auto erasePlayerIterator = std::find(m_playersList.begin(), m_playersList.end(), player);
        if(erasePlayerIterator != m_playersList.end())
        {
            m_playersList.erase(erasePlayerIterator);
            player->blockSignals(true);
            player->deleteLater();
        }
    }
}

void Arena::timerEvent(QTimerEvent*)
{
    if(m_foodList.size() < 1500)
        for(quint8 i = 0; i < 5; ++i)
            m_foodList << QPoint(rand() % (arenaSize - 60) + 30, rand() % (arenaSize - 60) + 30);

    if(m_trapList.size() < 200)
        m_trapList << QPoint(rand() % (arenaSize - 100) + 50, rand() % (arenaSize - 100) + 50);

    this->knockoutTest();
    for(auto playerIterator = m_playersList.begin(); playerIterator != m_playersList.end(); ++playerIterator)
    {
        //(*playerIterator)->decrease(1);
        (*playerIterator)->doStep();

        //пoглощение еды
        for(auto foodIterator = m_foodList.begin(); foodIterator != m_foodList.end(); ++foodIterator)
        {
            if(m_foodList.isEmpty())
            {
                break;
            }
            if((*playerIterator)->containsInRound(*foodIterator))
            {
                m_foodList.erase(foodIterator);
                (*playerIterator)->growUp(10);
                foodIterator = m_foodList.begin();
            }
        }

        //урон от ловушек
        for(auto trapIterator = m_trapList.begin(); trapIterator != m_trapList.end(); ++trapIterator)
        {
            if(m_trapList.isEmpty())
            {
                break;
            }
            if((*playerIterator)->weight() < 180)
            {
                break;
            }
            if((*playerIterator)->containsInRound(*trapIterator))
            {
                m_trapList.erase(trapIterator);
                (*playerIterator)->decrease(50);
                trapIterator = m_trapList.begin();
            }
        }

        //пересечения с другими игроками
        for(auto secondPlayerIterator = m_playersList.begin(); secondPlayerIterator != m_playersList.end(); ++secondPlayerIterator)
        {
            QPointF p1 = QPoint(7, 7) + (*secondPlayerIterator)->currentPosition() + QPoint((*secondPlayerIterator)->weight() / 20, (*secondPlayerIterator)->weight() / 20);
            QPointF p2 = QPoint(7, 7) + (*playerIterator)->currentPosition() + QPoint((*playerIterator)->weight() / 20, (*playerIterator)->weight() / 20);
            QPointF diff = p1 - p2;
            qreal distanceToCenters = sqrt(pow(diff.x(), 2) + pow(diff.y(), 2));
            qreal biggestPlayerRadius;
            if((*playerIterator)->weight() == (*secondPlayerIterator)->weight())
            {
                continue;
            }
            else if((*playerIterator)->weight() > (*secondPlayerIterator)->weight())
            {
                biggestPlayerRadius = (*playerIterator)->weight() / 20 + 7;
                if(distanceToCenters < biggestPlayerRadius)
                {
                    suckWeight((*playerIterator), (*secondPlayerIterator));
                }
            }
            else
            {
                biggestPlayerRadius = (*secondPlayerIterator)->weight() / 20 + 7;
                if(distanceToCenters < biggestPlayerRadius)
                {
                    suckWeight((*secondPlayerIterator), (*playerIterator));
                }
            }
        }
    }


    QByteArray dataBlock;
    QDataStream outStream(&dataBlock, QIODevice::OpenModeFlag::WriteOnly);
    outStream << User::ServerSignals::UpdateArena;

    outStream << quint16(m_playersList.size());
    for(auto playerIterator = m_playersList.begin(); playerIterator != m_playersList.end(); ++playerIterator)
    {
        outStream << (*playerIterator);
    }

    outStream << quint16(m_foodList.size());
    for(auto foodIterator = m_foodList.begin(); foodIterator != m_foodList.end(); ++foodIterator)
    {
        outStream << (*foodIterator);
    }

    outStream << quint16(m_trapList.size());
    for(auto trapIterator = m_trapList.begin(); trapIterator != m_trapList.end(); ++trapIterator)
    {
        outStream << (*trapIterator);
    }

    emit this->signalUpdateArena(dataBlock);
}

void Arena::knockoutTest()
{
    for(auto playerIterator = m_playersList.begin(); playerIterator != m_playersList.end(); ++playerIterator)
    {
        if(m_playersList.isEmpty())
        {
            break;
        }
        if((*playerIterator)->weight() <= 0)
        {
            (*playerIterator)->signalExitFromArena();
            m_playersList.erase(playerIterator);
            playerIterator = m_playersList.begin();
        }
    }
}
void Arena::suckWeight(Player* sucked, Player* sucking)
{
    sucked->growUp(5);
    sucking->decrease(5);
}
