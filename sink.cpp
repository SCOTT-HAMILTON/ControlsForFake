#include "sink.h"

Sink::Sink(const QString &name,
           const QString &description,
           int index, QObject *parent) :
    QObject(parent), m_name(name), m_description(description),
    m_index(index)
{
}

QString Sink::name() const
{
    return m_name;
}

QString Sink::description() const
{
    return m_description;
}

int Sink::index() const
{
    return m_index;
}
