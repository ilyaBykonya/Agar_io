#ifndef PLAYER_H
#define PLAYER_H
#include "User/User.h"
#include "SharedData.h"
#include <QtMath>
#include <QColor>
#include <QSizeF>

class Player : public QObject
{
    Q_OBJECT
private:
    QColor m_playerColor;
    QPointF m_position;
    QSizeF m_playerSpeed;
    User* m_user;
    quint32 m_weight;


    quint32 m_growUpCounter;

public:
    explicit Player(User*, QObject *parent = nullptr);
    quint32 weight() const;
    quint32 id() const;

    QPointF currentPosition() const;

    void doStep();
    bool containsInRound(QPoint);
    void growUp(quint32);
    void decrease(quint32);


    friend QDataStream& operator<<(QDataStream&, const Player*);
protected slots:
    void slotUserGoToThisPoint(QPoint);
    void slotUserExitFromGame();


signals:
    void signalUpdateArena(const QByteArray&);
    void signalExitFromArena();
    void singalUserDisconnected();
};

#endif // PLAYER_H
