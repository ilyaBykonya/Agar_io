#include "ArenaView.h"

ArenaView::ArenaView(QWidget *parent)
    :QWidget{ parent },
     m_arenaView{ new  QGraphicsView },
     m_arenaScene{ new QGraphicsScene(this) },
     m_arena{ new ArenaWidget },
     m_infoPanel{ new QLabel },
     m_scaleGrade{ 1.0 }
    {
        m_arenaView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        m_arenaView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);   m_arenaView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        m_arenaScene->setSceneRect(QRect({0, 0}, m_arena->size()));
        m_arenaScene->addWidget(m_arena);

        m_arenaView->setScene(m_arenaScene);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(m_arenaView, 7);
        layout->addWidget(m_infoPanel, 1);
        this->setLayout(layout);

        QObject::connect(this, &ArenaView::signalUpdateArena,
                         m_arena, &ArenaWidget::slotUpdateArena, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_arena, &ArenaWidget::signalPlayerGoToThisPoint,
                         this, &ArenaView::signalPlayerGoToThisPoint, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_arena, &ArenaWidget::signalPlayerWantExit,
                         this, &ArenaView::signalPlayerWantExit, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_arena, &ArenaWidget::signalUpdateInfo,
                         this, &ArenaView::slotUpdateInfo, Qt::ConnectionType::UniqueConnection);
    }

ArenaWidget* ArenaView::arena()
{
    return m_arena;
}

void ArenaView::slotUpdateInfo(quint32 id, QPoint pos, quint32 weight)
{
    m_arenaView->centerOn(pos.x(), pos.y());
    this->m_infoPanel->setText(QString("ID: %1\n"
                                       "Pos: [%2:%3]\n"
                                       "Weight: %4\n"
                                       "Viewport: [%5:%6]").arg(id).arg(pos.x()).arg(pos.y()).arg(weight).arg(this->m_arenaView->viewport()->width()).arg(this->m_arenaView->viewport()->height()));
}

void ArenaView::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() & Qt::KeyboardModifier::ControlModifier)
    {
        switch (event->key())
        {
            case Qt::Key::Key_Plus:
            {
                if(m_scaleGrade < 1.6)
                {
                    m_scaleGrade *= 1.1;
                    this->m_arenaView->scale(1.1, 1.1);
                }
                break;
            }
            case Qt::Key::Key_Minus:
            {
                if(m_scaleGrade > 0.6)
                {
                    m_scaleGrade *= (1.0 / 1.1);
                    this->m_arenaView->scale(1.0 / 1.1, 1.0 / 1.1);
                }
                break;
            }
        }
    }
}
