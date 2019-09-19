#include "sortserver.h"

#include <QtWebSockets>
#include <QtCore>

#include <cstdio>
using namespace std;

QT_USE_NAMESPACE

static QString getIdentifier(QWebSocket *peer)
{
    return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(),
                                       QString::number(peer->peerPort()));
}

SortServer::SortServer(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("SortServe"),
                                            QWebSocketServer::NonSecureMode,
                                            this))
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
	    printf("listen on %d\n",port);
        QTextStream(stdout) << "Sort Server listening on port " << port << '\n';
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &SortServer::onNewConnection);
    } else {
	    printf("listen failed \n");
    }
}

SortServer::~SortServer()
{
    m_pWebSocketServer->close();
}
//! [constructor]

//! [onNewConnection]
void SortServer::onNewConnection()
{
    auto pSocket = m_pWebSocketServer->nextPendingConnection();
    QTextStream(stdout) << getIdentifier(pSocket) << " connected!\n";
    pSocket->setParent(this);

    connect(pSocket, &QWebSocket::textMessageReceived,
            this, &SortServer::processMessage);
    connect(pSocket, &QWebSocket::disconnected,
            this, &SortServer::socketDisconnected);

    m_clients << pSocket;
}
//! [onNewConnection]

//! [processMessage]
void SortServer::processMessage(const QString &message)
{
	printf("Message:%s\n",message.toUtf8().constData());
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    for (QWebSocket *pClient : qAsConst(m_clients)) {
        if (pClient != pSender) //don't echo message back to sender
            pClient->sendTextMessage(message);
    }
}
//! [processMessage]

//! [socketDisconnected]
void SortServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QTextStream(stdout) << getIdentifier(pClient) << " disconnected!\n";
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
//! [socketDisconnected]
