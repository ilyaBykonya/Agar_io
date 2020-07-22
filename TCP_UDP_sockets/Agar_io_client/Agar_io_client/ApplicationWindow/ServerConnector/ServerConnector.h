#ifndef SERVERCONNECTOR_H
#define SERVERCONNECTOR_H
#include <QDataStream>
#include <QTcpSocket>
#include <QPoint>
#include <QFile>
#include <QSslKey>


class ServerConnector : public QObject
{
    Q_OBJECT
private:
    enum ServerSignals
    {
        SetID = 0,

        EnterToArena,
        LeaveArena
    };
    enum UserSignals
    {
        UserWantPlay = 0,
        UserWantExit,

        UserMoveToPoint
    };


    QTcpSocket* m_socket;
    quint16 m_nextDataBlockSize;
    quint32 m_ID;
public:
    explicit ServerConnector(const QString&, quint16, QObject *parent = nullptr);

public slots:
    void slotSendPlayerWantPlay();
    void slotSendPlayerGoToThisPoint(QPoint);
    void slotSendPlayerWantExit();

protected slots:
    void slotReadyRead();

    void slotSocketError(QAbstractSocket::SocketError);

signals:
    void signalEnterToArena(quint32);
    void signalLeaveArena();
};

#endif // SERVERCONNECTOR_H
