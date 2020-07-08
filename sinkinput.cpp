#include "sinkinput.h"

SinkInput::SinkInput(const QString &name,
                           int sink,
                           const QString &processBinaryName,
                           int index,
                           QObject *parent) :
    QObject(parent), m_name(name),
    m_processBinaryName(processBinaryName),
	m_sink(sink), m_index(index)
{
}

QString SinkInput::name() const
{
    return m_name;
}

int SinkInput::sink() const
{
    return m_sink;
}

QString SinkInput::processBinaryName() const
{
    return m_processBinaryName;
}

int SinkInput::index() const
{
    return m_index;
}
