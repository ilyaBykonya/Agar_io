#ifndef USER_H
#define USER_H
#include <QDataStream>
#include <QTcpSocket>
#include <QHostAddress>
#include <QPoint>
#include <QTimer>
class User : public QObject
{
    Q_OBJECT
public:
    enum ServerSignals
    {
        SetID = 0,

        EnterToArena
    };
    enum UserSignals
    {
        UserWantPlay = 0,
        UserWantExit,

        UserMoveToPoint
    };

private:
    quint16 m_nextDataBlockSize;
    QTcpSocket* m_socket;

    quint32 m_ID;
public:
    explicit User(quint32, QTcpSocket*, QObject* = nullptr);
    QHostAddress userIPAddress() const;
    quint32 id() const;

signals:
    void signalUserWantPlay(User*);
    void signalUserGoToThisPoint(QPoint);

    void signalUserDisconnected();
    void signalUserWantExit();

public slots:
    void slotReadyRead();
    void slotEnterToArena();
};

#endif // USER_H
