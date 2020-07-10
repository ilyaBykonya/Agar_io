#include "GameMenu.h"

GameMenu::GameMenu(QWidget *parent)
    :QWidget{ parent },
     m_playButton{ new QPushButton("Play") },
     m_exitButton{ new QPushButton("Quit") }
    {
        QGridLayout* layout = new QGridLayout;
        layout->addWidget(m_playButton, 2, 1, 1, 2);
        layout->addWidget(m_exitButton, 3, 1, 1, 2);
        for(int i = 0; i < 7; ++i)
            layout->setRowStretch(i, 1);
        for(int i = 0; i < 4; ++i)
            layout->setColumnStretch(i, 1);
        this->setLayout(layout);

        QObject::connect(m_playButton, &QPushButton::clicked,
                         this, &GameMenu::signalPlayButtonWasClicked, Qt::ConnectionType::UniqueConnection);
        QObject::connect(m_exitButton, &QPushButton::clicked,
                         this, &GameMenu::signalExitButtonWasClicked, Qt::ConnectionType::UniqueConnection);
    }




















