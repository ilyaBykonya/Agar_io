#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H
#include "Arena/ArenaView.h"
#include "Menu/GameMenu.h"
#include "ServerConnector/ServerConnector.h"
#include <QHBoxLayout>

class ApplicationWindow : public QWidget
{
    Q_OBJECT
private:
    ServerConnector* m_serverConnector;
    QHBoxLayout* m_layout;

    GameMenu* m_menu;
    ArenaView* m_arena;
public:
    explicit ApplicationWindow(const QString&, quint16, QWidget *parent = nullptr);

public slots:
    void transitionFromMenuToGame(quint16, const QByteArray&);
    void transitionFromGameToMenu();

signals:
    void signalExitButtonWasClicked(bool);
};

#endif // APPLICATIONWINDOW_H
