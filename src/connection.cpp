#include "connection.h"

#include <QHostAddress>
#include <QTimer>

#include "packets/packet.h"
#include "packets/packet_factory.h"

Connection::Connection(QObject* parent)
        : QTcpSocket(parent)
{
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(on_timeout()));

    connect(this, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(this, SIGNAL(readyRead()), this, SLOT(on_dataReceived()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error()));

    m_receiveState = ReceiveState::HEADER;
}

Connection::Connection(qintptr descriptor, QObject* parent)
        : Connection(parent)
{
    setSocketDescriptor(descriptor);
}

void Connection::connectToHost(const QString& hostName, quint16 port)
{
    if(hostName.length() == 0)
        QTcpSocket::connectToHost(QHostAddress::LocalHost, port);
    else
        QTcpSocket::connectToHost(hostName, port);

    m_timeoutTimer->start(TIMEOUT);
}

bool Connection::send(const Packet& packet)
{
    if(state() != QAbstractSocket::ConnectedState)
        return false;

    write(&HEADER_VAL, sizeof(HEADER_VAL));

    uint8_t packetId = packet.getId();
    write((char*) &packetId, sizeof(packetId));

    size_t size = packet.getSize();
    write((char*) &size, sizeof(size));

    char* data = packet.serialize();
    write(data, size);

    delete[] data;
    return true;
}

void Connection::on_connected()
{
    m_timeoutTimer->stop();
}

void Connection::on_dataReceived()
{
    if(m_processing)
        return;

    m_processing = true;
    while(bytesAvailable() > 0)
    {
        switch(m_receiveState)
        {
            case HEADER:
            {
                if(bytesAvailable() < 1)
                    return;

                char header;
                read(&header, 1);

                if(header == HEADER_VAL)
                    m_receiveState = ReceiveState::ID;

                break;
            }
            case ID:
            {
                if(bytesAvailable() < sizeof(int))
                    return;

                uint8_t packetId;
                read((char*) &packetId, sizeof(packetId));

                m_currentPacket = PacketFactory::Create(packetId);


                m_receiveState = ReceiveState::SIZE;

                break;
            }
            case SIZE:
            {
                if(bytesAvailable() < sizeof(size_t))
                    return;

                read((char*) &m_payloadSize, sizeof(m_payloadSize));

                m_receiveState = ReceiveState::PAYLOAD;

                break;
            }
            case PAYLOAD:
            {
                if(bytesAvailable() < m_payloadSize)
                    return;

                char data[m_payloadSize];
                read(data, m_payloadSize);

                m_currentPacket->deserialize(data);
                emit packetReceived(m_currentPacket);

                m_receiveState = ReceiveState::HEADER;

                break;
            }
            default:
                break;
        }
    }
    m_processing = false;
}

void Connection::on_timeout()
{
    setSocketError(SocketError::SocketTimeoutError);
    setErrorString("Socket timed out");
    emit error(SocketError::SocketTimeoutError);
}

void Connection::on_error()
{
    m_timeoutTimer->stop();
}

