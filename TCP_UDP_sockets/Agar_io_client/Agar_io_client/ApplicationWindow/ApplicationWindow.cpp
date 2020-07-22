#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow(const QString& hostName, quint16 portNumber, QWidget *parent)
    :QWidget{ parent },
     m_serverConnector{ new ServerConnector(hostName, portNumber, this) },
     m_layout{ new QHBoxLayout },
     m_menu{ nullptr },
     m_arena{ nullptr }
    {
        this->setLayout(m_layout);
        transitionFromGameToMenu();
    }


void ApplicationWindow::transitionFromMenuToGame(quint32 thisPlayerID)
{
    if(m_menu)
    {
        m_menu->hide();
        m_menu->deleteLater();
        m_menu = nullptr;
    }

    m_arena = new ArenaView(thisPlayerID);
    QObject::connect(m_arena, &ArenaView::signalPlayerGoToThisPoint,
                     m_serverConnector, &ServerConnector::slotSendPlayerGoToThisPoint, Qt::ConnectionType::UniqueConnection);
    QObject::connect(m_arena, &ArenaView::signalPlayerWantExit,
                     m_serverConnector, &ServerConnector::slotSendPlayerWantExit, Qt::ConnectionType::UniqueConnection);
    QObject::connect(m_arena, &ArenaView::signalPlayerWantExit,
                     this, &ApplicationWindow::transitionFromGameToMenu, Qt::ConnectionType::UniqueConnection);


    m_layout->addWidget(m_arena);

    this->resize(600, 600);

    QObject::connect(m_serverConnector, &ServerConnector::signalLeaveArena,
                     this, &ApplicationWindow::transitionFromGameToMenu, Qt::ConnectionType::UniqueConnection);
    QObject::disconnect(m_serverConnector, &ServerConnector::signalEnterToArena,
                     this, &ApplicationWindow::transitionFromMenuToGame);

}
void ApplicationWindow::transitionFromGameToMenu()
{
    if(m_arena)
    {
        m_arena->hide();
        m_arena->deleteLater();
        m_arena = nullptr;
    }

    m_menu = new GameMenu;
    QObject::connect(m_menu, &GameMenu::signalPlayButtonWasClicked,
                     m_serverConnector, &ServerConnector::slotSendPlayerWantPlay, Qt::ConnectionType::UniqueConnection);
    QObject::connect(m_menu, &GameMenu::signalExitButtonWasClicked,
                     this, &ApplicationWindow::signalExitButtonWasClicked, Qt::ConnectionType::UniqueConnection);
    m_layout->addWidget(m_menu);

    this->resize(180, 270);

    QObject::disconnect(m_serverConnector, &ServerConnector::signalLeaveArena,
                     this, &ApplicationWindow::transitionFromGameToMenu);
    QObject::connect(m_serverConnector, &ServerConnector::signalEnterToArena,
                     this, &ApplicationWindow::transitionFromMenuToGame, Qt::ConnectionType::UniqueConnection);
}
