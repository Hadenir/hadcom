#include "serializer.h"

Serializer::Serializer(char* data)
        : m_data(data)
{}

void Serializer::serializeField(char field)
{
    size_t length = sizeof(char);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(signed char field)
{
    size_t length = sizeof(signed char);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(unsigned char field)
{
    size_t length = sizeof(unsigned char);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(short field)
{
    size_t length = sizeof(short);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(unsigned short field)
{
    size_t length = sizeof(unsigned short);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(int field)
{
    size_t length = sizeof(int);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(unsigned int field)
{
    size_t length = sizeof(unsigned int);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(long field)
{
    size_t length = sizeof(long);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(unsigned long field)
{
    size_t length = sizeof(unsigned long);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(long long field)
{
    size_t length = sizeof(long long);
    memcpy(&m_data[m_idx], &field, length);
    m_idx += length;
}

void Serializer::serializeField(unsigned long long field)
{
    size_t length = sizeof(unsigned long long);
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
        serializeField(byte);
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

