#include "deserializer.h"

Deserializer::Deserializer(const char* data)
        : m_data(data)
{}

void Deserializer::deserializeField(char& field)
{
    size_t length = sizeof(char);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(signed char& field)
{
    size_t length = sizeof(signed char);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(unsigned char& field)
{
    size_t length = sizeof(unsigned char);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(short& field)
{
    size_t length = sizeof(short);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(unsigned short& field)
{
    size_t length = sizeof(unsigned short);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(int& field)
{
    size_t length = sizeof(int);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(unsigned int& field)
{
    size_t length = sizeof(unsigned int);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(long& field)
{
    size_t length = sizeof(long);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(unsigned long& field)
{
    size_t length = sizeof(unsigned long);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(long long& field)
{
    size_t length = sizeof(long long);
    memcpy(&field, &m_data[m_idx], length);
    m_idx += length;
}

void Deserializer::deserializeField(unsigned long long& field)
{
    size_t length = sizeof(unsigned long long);
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

    char byte;
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
