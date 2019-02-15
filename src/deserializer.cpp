#include "deserializer.h"

Deserializer::Deserializer(const char* data)
        : m_data(data)
{}

void Deserializer::deserializeField(int8_t& field)
{
    size_t length = sizeof(int8_t);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(uint8_t& field)
{
    size_t length = sizeof(uint8_t);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(int16_t& field)
{
    size_t length = sizeof(int16_t);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(uint16_t& field)
{
    size_t length = sizeof(uint16_t);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(int32_t& field)
{
    size_t length = sizeof(int32_t);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(uint32_t& field)
{
    size_t length = sizeof(uint32_t);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(int64_t& field)
{
    size_t length = sizeof(int64_t);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(uint64_t& field)
{
    size_t length = sizeof(uint64_t);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(std::string& field)
{
    std::string string(&m_data[m_idx]);
    field = string;
    m_idx += string.length() + 1;
}

void Deserializer::deserializeField(QByteArray& field)
{
    int size;
    deserializeField(size);

    QByteArray byteArray;
    byteArray.reserve(size);

    uint8_t byte;
    for(int i = 0; i < size; ++i)
    {
        deserializeField(byte);
        byteArray.push_back(byte);
    }

    field = byteArray;
}
#include <QDebug>
void Deserializer::deserializeField(QString& field)
{
    QByteArray bytes;
    deserializeField(bytes);

    QString string = QString::fromUtf8(bytes);
    field = string;
}

void Deserializer::deserializeField(QList<QString>& field)
{
    int size;
    deserializeField(size);

    QList<QString> list;
    list.reserve(size);

    QString string;
    for(int i = 0; i < size; ++i)
    {
        deserializeField(string);
        list.push_back(string);
    }

    field = list;
}
