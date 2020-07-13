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

public:
    explicit Arena(QObject* = nullptr);

    quint32 amountOfPlayersInInstance() const;

public slots:
    void slotConnectUser(User*);

protected slots:
    void slotDisconnectUser();
    virtual void timerEvent(QTimerEvent*) override;

protected:
    void knockoutTest();//проверка на выбывание
    void eatingTest();//поглощение еды
    void trapsTest();//попадание в ловушки
    void absorptionTest();//поглощение игроками друг-друга
    void suckWeight(Player*, Player*);



    void updateArena();//отправка игрокам информации об арене

signals:
    void signalUpdateArena(const QByteArray&);
};

#endif // ARENA_H
