#ifndef ARENA_H
#define ARENA_H
#include "ArenaWidget/ArenaWidget.h"
/*
 * Я использовал MVC (медель-вид-контроллер),
 * т.к. мне нужно было обеспечить:
 * а) центрирование на игроке
 * б) показ только части карты
 * в) зум
 */
class ArenaView : public QWidget
{
    Q_OBJECT
private:
    QGraphicsView* m_arenaView;
    QGraphicsScene* m_arenaScene;
    ArenaWidget* m_arena;

    QLabel* m_infoPanel;

    //эта переменная хранит текущее значение зума, а так же
    //ограничивает его
    qreal m_zoomLevel;

public:
    explicit ArenaView(QWidget *parent = nullptr);
    ArenaWidget* arena();

    void slotUpdateInfo(quint32, QPoint, quint32);

protected slots:
    virtual void keyPressEvent(QKeyEvent *event) override;

signals:
    void signalUpdateArena(quint32, QDataStream&);

    void signalPlayerGoToThisPoint(QPoint);
    void signalPlayerWantExit();
};

#endif // ARENA_H
