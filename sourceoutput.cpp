#include "sourceoutput.h"

SourceOutput::SourceOutput(const QString &name,
                           const QString &source,
                           const QString &sourceProcessBinaryName,
                           int index,
                           QObject *parent) :
    QObject(parent), m_name(name), m_source(source),
    m_sourceProcessBinaryName(sourceProcessBinaryName), m_index(index)
{
}

QString SourceOutput::name() const
{
    return m_name;
}

QString SourceOutput::source() const
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
