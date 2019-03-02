#pragma once

#include <QTcpSocket>

#include "packets/packet.h"

class Connection : public QTcpSocket
{
Q_OBJECT

public:
    explicit Connection(QObject* parent = nullptr);

    explicit Connection(qintptr descriptor, QObject* parent = nullptr);

    void connectToHost(const QString& hostName, quint16 port);

    bool send(const Packet& packet);

signals:
    void packetReceived(Packet* packet);

private slots:
    void on_dataReceived();

private:
    const char HEADER_VAL = (char)0xBF;

    enum ReceiveState
    {
        HEADER,
        ID,
        SIZE,
        PAYLOAD
    } m_receiveState;

    bool m_processing = false;

    size_t m_payloadSize = 0;

    Packet* m_currentPacket = nullptr;
};