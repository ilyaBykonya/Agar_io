#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow(const QUrl& hostAddress, QWidget *parent)
    :QWidget{ parent },
     m_serverConnector{ new ServerConnector(hostAddress, this) },
     m_layout{ new QHBoxLayout },
     m_menu{ new GameMenu },
     m_arena{ new ArenaView }
    {
        this->setLayout(m_layout);
        m_layout->addWidget(m_menu);
        m_layout->addWidget(m_arena);


        QObject::connect(m_menu, &GameMenu::signalPlayButtonWasClicked,
                         m_serverConnector, &ServerConnector::slotSendPlayerWantPlay, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_menu, &GameMenu::signalExitButtonWasClicked,
                         this, &ApplicationWindow::signalExitButtonWasClicked, Qt::ConnectionType::UniqueConnection);




        QObject::connect(m_serverConnector, &ServerConnector::signalUpdateArena,
                         m_arena, &ArenaView::signalUpdateArena, Qt::ConnectionType::UniqueConnection);

        QObject::connect(m_arena, &ArenaView::signalPlayerGoToThisPoint,
                         m_serverConnector, &ServerConnector::slotSendPlayerGoToThisPoint, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_arena, &ArenaView::signalPlayerWantExit,
                         m_serverConnector, &ServerConnector::slotSendPlayerWantExit, Qt::ConnectionType::UniqueConnection);





        transitionFromGameToMenu();
    }


void ApplicationWindow::transitionFromMenuToGame(quint16 thisPlayerID, QDataStream& inStream)
{
    m_menu->hide();
    m_menu->blockSignals(true);
    QObject::disconnect(m_serverConnector, &ServerConnector::signalUpdateArena,
                     this, &ApplicationWindow::transitionFromMenuToGame);
    QObject::connect(m_serverConnector, &ServerConnector::signalExitFromArena,
                     this, &ApplicationWindow::transitionFromGameToMenu, Qt::ConnectionType::UniqueConnection);

    m_arena->blockSignals(false);
    m_arena->show();
    m_arena->arena()->show();
    this->resize(600, 600);
    m_arena->signalUpdateArena(thisPlayerID, inStream);

}
void ApplicationWindow::transitionFromGameToMenu()
{
    m_arena->hide();
    m_arena->blockSignals(true);

    QObject::disconnect(m_serverConnector, &ServerConnector::signalExitFromArena,
                        this, &ApplicationWindow::transitionFromGameToMenu);
    QObject::connect(m_serverConnector, &ServerConnector::signalUpdateArena,
                     this, &ApplicationWindow::transitionFromMenuToGame, Qt::ConnectionType::UniqueConnection);

    m_menu->blockSignals(false);
    m_menu->show();
    this->resize(180, 270);
}
