#pragma once

#include <QTcpServer>
#include <QMap>
#include <QList>

class Packet;
class Connection;
class RemoteClient;

class Server : public QTcpServer
{
Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);

    bool bind(quint16 port);

    bool broadcast(const Packet& packet) const;

    QList<QString> getClientsList() const;

protected:
    void incomingConnection(qintptr descriptor) override;

private slots:
    void on_packetReceived(Packet* packet);

    void on_connectionLost();

private:
    const int MAGIC = 0x12052000;

    QMap<Connection*, RemoteClient*> m_clients;
};