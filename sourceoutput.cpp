#include "sourceoutput.h"

SourceOutput::SourceOutput(const QString &name,
                           int source,
                           const QString &processBinaryName,
                           int index,
                           QObject *parent) :
    QObject(parent), m_name(name),
    m_processBinaryName(processBinaryName),
	m_source(source), m_index(index)
{
}

QString SourceOutput::name() const
{
    return m_name;
}

int SourceOutput::source() const
{
    return m_source;
}

QString SourceOutput::processBinaryName() const
{
    return m_processBinaryName;
}

int SourceOutput::index() const
{
    return m_index;
}
