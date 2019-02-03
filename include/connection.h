#pragma once

#include <QTcpSocket>

class QTimer;
class Packet;

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
    void on_connected();

    void on_dataReceived();

    void on_timeout();

    void on_error();

private:
    const char HEADER_VAL = (char)0xBF;

    const int TIMEOUT = 2500;

    enum ReceiveState
    {
        HEADER,
        ID,
        SIZE,
        PAYLOAD,
        CHECKSUM
    } m_receiveState;

    bool m_processing = false;

    size_t m_payloadSize = 0;

    Packet* m_currentPacket = nullptr;

    char m_currentChecksum = 0;

    QTimer* m_timeoutTimer;
};