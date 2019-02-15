#include "serializer.h"

Serializer::Serializer(char* data)
        : m_data(data)
{}

void Serializer::serializeField(int8_t field)
{
    size_t length = sizeof(int8_t);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(uint8_t field)
{
    size_t length = sizeof(uint8_t);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(int16_t field)
{
    size_t length = sizeof(int16_t);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(uint16_t field)
{
    size_t length = sizeof(uint16_t);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(int32_t field)
{
    size_t length = sizeof(int32_t);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(uint32_t field)
{
    size_t length = sizeof(uint32_t);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(int64_t field)
{
    size_t length = sizeof(int64_t);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(uint64_t field)
{
    size_t length = sizeof(uint64_t);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(const std::string& field)
{
    size_t length = field.length() + 1;
    const char* str = field.c_str();
    memcpy(&m_data[m_idx], str, length);
    m_idx += length;
}

void Serializer::serializeField(const QByteArray& field)
{
    serializeField(field.size());
    for(auto byte : field)
        serializeField((uint8_t)byte);
}

void Serializer::serializeField(const QString& field)
{
    QByteArray utf8 = field.toUtf8();
    serializeField(utf8);
}

void Serializer::serializeField(const QList<QString>& field)
{
    serializeField(field.size());
    for(const auto& string : field)
        serializeField(string);
}

