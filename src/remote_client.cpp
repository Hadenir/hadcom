#include "remote_client.h"

RemoteClient::RemoteClient(QObject* parent)
        : QObject(parent)
{}

void RemoteClient::setNickname(const QString& nickname)
{
    m_nickname = nickname;
}

QString RemoteClient::getNickname() const
{
    return m_nickname;
}