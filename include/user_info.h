#pragma once

#include <QString>

enum class Mode
{
    CLIENT,
    SERVER
};

struct UserInfo
{
    Mode mode;
    QString nickname;
    QString address;
    quint16 port;
};