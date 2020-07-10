#ifndef ARENA_H
#define ARENA_H
#include "ArenaWidget/ArenaWidget.h"
class ArenaView : public QWidget
{
    Q_OBJECT
private:
    QGraphicsView* m_arenaView;
    QGraphicsScene* m_arenaScene;
    ArenaWidget* m_arena;


    QLabel* m_infoPanel;

    qreal m_scaleGrade;
public:
    explicit ArenaView(QWidget *parent = nullptr);
    ArenaWidget* arena();


    void slotUpdateCameraPosition(QPoint);
    void slotUpdateInfo(quint32, QPoint, quint32);

protected slots:
    virtual void keyPressEvent(QKeyEvent *event) override;

signals:
    void signalUpdateArena(quint32, const QByteArray&);

    void signalPlayerGoToThisPoint(QPoint);
    void signalPlayerWantExit();
};

#endif // ARENA_H
