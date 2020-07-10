#ifndef SERVERCONNECTOR_H
#define SERVERCONNECTOR_H
#include <QDataStream>
#include <QWebSocket>
#include <QPoint>

class ServerConnector : public QObject
{
    Q_OBJECT
private:
    enum ServerSignals
    {
        SetID = 0,
        UpdateArena,
        LeaveArena
    };
    enum UserActions
    {
        UserWandPlay = 0,
        UserMoveToPoint,
        UserWantExit
    };
    QWebSocket* m_webSocket;
    quint32 m_ID;
public:
    explicit ServerConnector(const QString&, quint16, QObject *parent = nullptr);

public slots:
    void slotConnection();
    void slotReadyRead(const QByteArray&);

    void slotSendPlayerWantPlay();
    void slotSendPlayerGoToThisPoint(QPoint);
    void slotSendPlayerWantExit();

signals:
    void signalUpdateArena(quint32, const QByteArray&);
    void signalExitFromArena();
};

#endif // SERVERCONNECTOR_H
