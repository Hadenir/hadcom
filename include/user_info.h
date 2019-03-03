#pragma once

#include <QString>

struct UserInfo
{
    QString nickname;
    QString address;
    quint16 port;
    bool server;
};