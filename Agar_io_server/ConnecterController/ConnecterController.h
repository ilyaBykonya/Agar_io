#ifndef CONNECTERCONNECT_H
#define CONNECTERCONNECT_H
#include <QWebSocket>
#include <QWebSocketServer>
#include <QTextEdit>
#include <QMessageBox>
#include "Arena/Arena.h"
#include "User/User.h"
class ConnecterController : public QTextEdit
{
    Q_OBJECT
private:
    QWebSocketServer* m_server;
    QList<Arena*> m_instancesList;
    quint32 m_currentUserID;
    quint32 m_currentArenaID;

public:
    explicit ConnecterController(quint16, QWidget *parent = nullptr);

public slots:
    void slotNewConnection();
    
    void slotUserWantPlay(User*);
};

#endif // CONNECTERCONNECT_H
