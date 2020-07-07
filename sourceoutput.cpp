#include "sourceoutput.h"

SourceOutput::SourceOutput(const QString &name,
                           int source,
                           const QString &sourceProcessBinaryName,
                           int index,
                           QObject *parent) :
    QObject(parent), m_name(name),
    m_sourceProcessBinaryName(sourceProcessBinaryName),
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

QString SourceOutput::sourceProcessBinaryName() const
{
    return m_sourceProcessBinaryName;
}

int SourceOutput::index() const
{
    return m_index;
}
