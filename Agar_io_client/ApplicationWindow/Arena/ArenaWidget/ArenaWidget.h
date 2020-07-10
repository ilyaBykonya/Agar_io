#ifndef ARENAWIDGET_H
#define ARENAWIDGET_H
#include <QtWidgets>

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

public:
    explicit ArenaWidget(QWidget *parent = nullptr);

public slots:
    void slotUpdateArena(quint32, const QByteArray&);

protected slots:
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void paintEvent(QPaintEvent*) override;

signals:
    void signalPlayerGoToThisPoint(QPoint);
    void signalPlayerWantExit();

    void signalUpdateInfo(quint32, QPoint, quint32);
};

#endif // ARENAWIDGET_H
