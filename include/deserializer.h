#pragma once

#include <cstddef>
#include <string>
#include <QString>
#include <QList>

class Deserializer
{
public:
    Deserializer(const char* data);

    void deserializeField(char& field);
    void deserializeField(signed char& field);
    void deserializeField(unsigned char& field);
    void deserializeField(short& field);
    void deserializeField(unsigned short& field);
    void deserializeField(int& field);
    void deserializeField(unsigned int& field);
    void deserializeField(long& field);
    void deserializeField(unsigned long& field);
    void deserializeField(long long& field);
    void deserializeField(unsigned long long& field);
    void deserializeField(std::string& field);
    void deserializeField(QString& field);
    void deserializeField(QList<QString>& field);

private:
    const char* m_data;

    size_t m_idx = 0;
};