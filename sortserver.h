#pragma once
#include <QtCore/QObject>
#include <QtCore/QList>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QString)

class SortServer : public QObject
{
    Q_OBJECT
public:
    explicit SortServer(quint16 port, QObject *parent = nullptr);
    virtual ~SortServer();

private slots:
    void onNewConnection();
    void processMessage(const QString &message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};

