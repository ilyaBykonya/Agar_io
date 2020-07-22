#ifndef ARENAWIDGET_H
#define ARENAWIDGET_H
#include <QtWidgets>
#include <QUdpSocket>
#include <QWidget>
#include <QPainter>
#include "sharedConstants.h"
#include <QPushButton>
struct Player
{
    quint32 m_weight;
    QPoint m_currentPosition;
    QColor m_playerColor;
};


class ArenaWidget : public QWidget
{
    Q_OBJECT

private:
    QList<Player> m_allPlayersList;
    QList<QPoint> m_foodList;
    QList<QPoint> m_trapList;
    QPixmap m_trapPicture;

    QUdpSocket* m_UDPSocket;
    quint32 m_thisPlayerID;
public:
    explicit ArenaWidget(quint32, QWidget *parent = nullptr);

public slots:
    void slotUpdateArena();

protected slots:
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void paintEvent(QPaintEvent*) override;

signals:
    void signalPlayerGoToThisPoint(QPoint);
    void signalUpdateInfo(QPoint, quint32);
};

#endif // ARENAWIDGET_H
