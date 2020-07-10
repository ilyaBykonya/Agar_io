#ifndef GAMEMENU_H
#define GAMEMENU_H
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
class GameMenu: public QWidget
{
    Q_OBJECT
private:
    QPushButton* m_playButton;
    QPushButton* m_exitButton;

public:
    explicit GameMenu(QWidget *parent = nullptr);

signals:
    void signalPlayButtonWasClicked(bool);
    void signalExitButtonWasClicked(bool);
};

#endif // GAMEMENU_H
