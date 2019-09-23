#include "sortserver.h"
#include "cpuMergesort.h"

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
	QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
	QJsonObject jsonObject = jsonResponse.object();
	
	QStringList list = jsonObject["original"].toString().split(",",QString::SkipEmptyParts);
	qDebug() << "Request:" <<jsonObject["original"].toString();
	int *numbers = (int *)malloc(list.count()*sizeof(int));
	int *sortnumbers = (int *)malloc(list.count()*sizeof(int));

	for(int index=0; index < list.count(); index++) {
		 numbers[index] = list[index].toInt();
	}

	mergesort_cpu(numbers, sortnumbers, list.count());

	QStringList answerList;
	for(int index=0; index < list.count(); index++) {
		 answerList << QString::number(sortnumbers[index]);
	}
	
	QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
	QString response = answerList.join(",");
	pSender->sendTextMessage(response);
	qDebug() << "Response:" << response;

	free(numbers);
	free(sortnumbers);
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
