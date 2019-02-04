#pragma once

#include <cstddef>
#include <string>
#include <QString>
#include <QList>

class Serializer
{
public:
    explicit Serializer(char* data);

    void serializeField(char field);
    void serializeField(signed char field);
    void serializeField(unsigned char field);
    void serializeField(short field);
    void serializeField(unsigned short field);
    void serializeField(int field);
    void serializeField(unsigned int field);
    void serializeField(long field);
    void serializeField(unsigned long field);
    void serializeField(long long field);
    void serializeField(unsigned long long field);
    void serializeField(const std::string& field);
    void serializeField(const QString& field);
    void serializeField(const QList<QString>& field);


private:
    char* m_data;

    size_t m_idx = 0;
};