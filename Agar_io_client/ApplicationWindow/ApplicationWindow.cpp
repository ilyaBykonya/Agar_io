#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow(const QString& hostName, quint16 portNumber, QWidget *parent)
    :QWidget{ parent },
     m_serverConnector{ new ServerConnector(hostName, portNumber, this) },
     m_layout{ new QHBoxLayout },
     m_menu{ new GameMenu },
     m_arena{ new ArenaView }
    {
        this->setLayout(m_layout);
        m_layout->addWidget(m_menu);
        m_layout->addWidget(m_arena);

        m_arena->blockSignals(true);
        m_arena->hide();

        QObject::connect(m_menu, &GameMenu::signalPlayButtonWasClicked,
                         m_serverConnector, &ServerConnector::slotSendPlayerWantPlay, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_menu, &GameMenu::signalExitButtonWasClicked,
                         this, &ApplicationWindow::signalExitButtonWasClicked, Qt::ConnectionType::UniqueConnection);



        //connect arena to server_connector
        QObject::connect(m_serverConnector, &ServerConnector::signalUpdateArena,
                         m_arena, &ArenaView::signalUpdateArena, Qt::ConnectionType::UniqueConnection);

        QObject::connect(m_arena, &ArenaView::signalPlayerGoToThisPoint,
                         m_serverConnector, &ServerConnector::slotSendPlayerGoToThisPoint, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_arena, &ArenaView::signalPlayerWantExit,
                         m_serverConnector, &ServerConnector::slotSendPlayerWantExit, Qt::ConnectionType::UniqueConnection);





        QObject::connect(m_serverConnector, &ServerConnector::signalUpdateArena,
                         this, &ApplicationWindow::transitionFromMenuToGame, Qt::ConnectionType::UniqueConnection);
        this->setFixedSize(180, 270);
    }


void ApplicationWindow::transitionFromMenuToGame(quint16, const QByteArray&)
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
    this->setFixedSize(700, 700);

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
    this->setFixedSize(180, 270);
}

