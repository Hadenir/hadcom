#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <QString>
#include <QList>

class Serializer
{
public:
    explicit Serializer(char* data);

    void serializeField(int8_t field);
    void serializeField(uint8_t field);
    void serializeField(int16_t field);
    void serializeField(uint16_t field);
    void serializeField(int32_t field);
    void serializeField(uint32_t field);
    void serializeField(int64_t field);
    void serializeField(uint64_t field);
    void serializeField(const std::string& field);
    void serializeField(const QByteArray& field);
    void serializeField(const QString& field);
    void serializeField(const QList<QString>& field);

private:
    char* m_data;

    size_t m_idx = 0;
};