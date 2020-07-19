#ifndef PLAYER_H
#define PLAYER_H
#include "User/User.h"
#include "SharedConstants.h"
#include <QMessageBox>
#include <QUdpSocket>
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

    QUdpSocket* m_arenaSocket;
public:
    explicit Player(QPoint, User*, QObject *parent = nullptr);
    ~Player();

    quint32 weight() const;
    quint32 id() const;

    QPointF currentPosition() const;
    QPointF centerThisUserPoint() const;

    void doStep();
    bool containsInRound(QPoint);
    void growUp(quint32);
    void decrease(quint32);


    friend QDataStream& operator<<(QDataStream&, const Player*);

public slots:
    void slotUpdateArena(const QByteArray&);

protected slots:
    void slotUserGoToThisPoint(QPoint);
    void slotUserExitFromGame();
};

#endif // PLAYER_H
