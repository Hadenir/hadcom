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

    char checksum = 0;

    int packetId = packet.getId();
    write((char*) &packetId, sizeof(packetId));

    for(int i = 0; i < sizeof(packetId); ++i)
        checksum ^= ((char*)&packetId)[i];

    size_t size = packet.getSize();
    write((char*) &size, sizeof(size));

    for(int i = 0; i < sizeof(size); ++i)
        checksum ^= ((char*)&size)[i];

    char* data = packet.serialize();
    write(data, packet.getSize());

    for(int i = 0; i < size; ++i)
        checksum ^= data[i];

    write(&checksum, 1);

    delete[] data;
    return true;
}

void Connection::on_connected()
{
    m_timeoutTimer->stop();
}

void Connection::on_dataReceived()
{
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

                int packetId;
                read((char*) &packetId, sizeof(packetId));

                m_currentPacket = PacketFactory::Create(packetId);

                m_currentChecksum = 0;
                for(int i = 0; i < sizeof(packetId); ++i)
                    m_currentChecksum ^= ((char*)&packetId)[i];

                m_receiveState = ReceiveState::SIZE;

                break;
            }
            case SIZE:
            {
                if(bytesAvailable() < sizeof(size_t))
                    return;

                read((char*) &m_payloadSize, sizeof(m_payloadSize));

                for(int i = 0; i < sizeof(m_payloadSize); ++i)
                    m_currentChecksum ^= ((char*)&m_payloadSize)[i];

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

                for(int i = 0; i < m_payloadSize; ++i)
                    m_currentChecksum ^= data[i];

                m_receiveState = ReceiveState::CHECKSUM;

                break;
            }
            case CHECKSUM:
            {
                if(bytesAvailable() < sizeof(m_currentChecksum))
                    return;

                char checksum;
                read(&checksum, 1);

                if(checksum == m_currentChecksum)
                    emit packetReceived(m_currentPacket);

                m_currentPacket = nullptr;
                m_currentChecksum = 0;
                m_payloadSize = 0;
                m_receiveState = ReceiveState::HEADER;

                break;
            }
            default:
                break;
        }
    }
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

