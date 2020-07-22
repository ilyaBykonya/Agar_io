#include "ArenaView.h"

ArenaView::ArenaView(quint32 thisPlayerID, QWidget *parent)
    :QWidget{ parent },
     m_arenaView{ new  QGraphicsView },
     m_arenaScene{ new QGraphicsScene(this) },
     m_arena{ new ArenaWidget(thisPlayerID) },
     m_infoPanel{ new QLabel },
     m_zoomLevel{ 1.0 }
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

        QObject::connect(m_arena, &ArenaWidget::signalUpdateInfo,
                         this, &ArenaView::slotUpdateInfo, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_arena, &ArenaWidget::signalPlayerGoToThisPoint,
                         this, &ArenaView::signalPlayerGoToThisPoint, Qt::ConnectionType::UniqueConnection);
    }

ArenaWidget* ArenaView::arena()
{
    return m_arena;
}

void ArenaView::slotUpdateInfo(QPoint pos, quint32 weight)
{
    m_arenaView->centerOn(pos.x(), pos.y());
    this->m_infoPanel->setText(QString("Pos   : [%1:%2]\n"
                                       "Weight:  %3\n"
                                       "Zoom  :  %4").arg(pos.x()).arg(pos.y()).arg(weight).arg(m_zoomLevel));
}
void ArenaView::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() & Qt::KeyboardModifier::ControlModifier)
    {
        switch (event->key())
        {
            case Qt::Key::Key_Plus:
            {
                if(m_zoomLevel < maxZoom)
                {
                    m_zoomLevel *= 1.1;
                    this->m_arenaView->scale(1.1, 1.1);
                }
                break;
            }
            case Qt::Key::Key_Minus:
            {
                if(m_zoomLevel > minZoom)
                {
                    m_zoomLevel *= (1.0 / 1.1);
                    this->m_arenaView->scale(1.0 / 1.1, 1.0 / 1.1);
                }
                break;
            }
        }
    }
    else if(event->key() & Qt::Key::Key_Escape)
    {
        emit this->signalPlayerWantExit();
    }
}

