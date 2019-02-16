#pragma once

#include "remote_client.h"

class Connection;
class Packet;

class LocalClient : public RemoteClient
{
Q_OBJECT

public:
    explicit LocalClient(Connection* connection, QObject* parent = nullptr);

    void sendMessage(const QString& message);

signals:
    void connected();

    void loggedIn(const QList<QString>& nicknames);

    void clientJoined(QString nickname);

    void clientLeft(QString nickname);

    void messageReceived(QString sender, QString message);

private slots:
    void on_connected();

    void on_packetReceived(Packet* packet);

private:
    const uint32_t MAGIC = 0x12052000;

    Connection* m_connection;
};
