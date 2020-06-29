#include "source.h"

Source::Source(const QString &name,
			   const QString &description,
			   int index, 
			   QObject *parent) :
    QObject(parent), m_name(name), m_description(description),
    m_index(index)
{
}

QString Source::name() const
{
    return m_name;
}

QString Source::description() const
{
    return m_description;
}

int Source::index() const
{
    return m_index;
}
