#ifndef ARENA_H
#define ARENA_H
#include <QList>
#include <QPoint>
#include "Player/Player.h"

class Arena : public QObject
{
    Q_OBJECT
private:
    QList<QPoint> m_foodList;
    QList<QPoint> m_trapList;
    QList<Player*> m_playersList;

    quint32 m_instanceID;

public:
    explicit Arena(quint32, QObject* = nullptr);

    quint32 amountOfPlayersInInstance() const;
    quint32 arenaID() const;

public slots:
    void slotConnectUser(User*);

protected:
    void knockoutTest();
    void suckWeight(Player*, Player*);

protected slots:
    void slotDisconnectUser();
    virtual void timerEvent(QTimerEvent*) override;

signals:
    void signalUpdateArena(const QByteArray&);
};

#endif // ARENA_H
