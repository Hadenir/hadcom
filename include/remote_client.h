#pragma once

#include <QObject>

class RemoteClient : public QObject
{
Q_OBJECT

public:
    explicit RemoteClient(QObject* parent = nullptr);

    void setNickname(const QString& nickname);

    QString getNickname() const;

private:
    QString m_nickname;
};
