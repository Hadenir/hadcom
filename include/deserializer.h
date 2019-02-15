#pragma once

#include <cstddef>
#include <string>
#include <QString>
#include <QList>

class Deserializer
{
public:
    explicit Deserializer(const char* data);

    void deserializeField(int8_t& field);
    void deserializeField(uint8_t& field);
    void deserializeField(int16_t& field);
    void deserializeField(uint16_t& field);
    void deserializeField(int32_t& field);
    void deserializeField(uint32_t& field);
    void deserializeField(int64_t& field);
    void deserializeField(uint64_t& field);
    void deserializeField(std::string& field);
    void deserializeField(QByteArray& field);
    void deserializeField(QString& field);
    void deserializeField(QList<QString>& field);

private:
    const char* m_data;

    size_t m_idx = 0;
};