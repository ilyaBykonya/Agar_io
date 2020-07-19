#ifndef CONNECTERCONNECT_H
#define CONNECTERCONNECT_H
#include "Arena/Arena.h"
#include <QMessageBox>
#include "User/User.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QSslKey>
#include <QFile>


class ConnecterController : public QTextEdit
{
    Q_OBJECT
private:
    QTcpServer* m_server;
    Arena* m_arena;

    quint32 m_currentUserID;
public:
    explicit ConnecterController(quint16, QWidget *parent = nullptr);

protected slots:
    void slotNewConnection();
    void slotUserWantPlay(User*);
    void slotUserDisconnected();
};

#endif // CONNECTERCONNECT_H
