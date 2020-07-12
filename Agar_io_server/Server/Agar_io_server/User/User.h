#ifndef USER_H
#define USER_H
#include <QDataStream>
#include <QWebSocket>
#include <QPoint>
#include <QTimer>
class User : public QObject
{
    Q_OBJECT
public:
    enum ServerSignals
    {
        SetID = 0,
        UpdateArena,
        LeaveArena
    };
    enum UserActions
    {
        UserWantPlay = 0,
        UserMoveToPoint,
        UserWantExit
    };

private:
    enum UserState
    {
        UserInLobbi = 0,
        UserInGame
    };

    QWebSocket* m_socket;
    quint32 m_ID;
    UserState m_state;
public:
    explicit User(quint32, QWebSocket*, QObject* = nullptr);
    quint32 id() const;

signals:
    void signalUserWantPlay(User*);
    void signalUserWantExit();
    void signalUserDisconnected();

    void signalUserGoToThisPoint(QPoint);

public slots:
    void slotReadyRead(const QByteArray&);

    void slotSendUpdateGameInstance(const QByteArray&);
    void slotExitFromArena();
};

#endif // USER_H
