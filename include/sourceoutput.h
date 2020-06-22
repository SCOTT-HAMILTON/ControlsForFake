#ifndef SOURCEOUTPUT_H
#define SOURCEOUTPUT_H

#include <QObject>

class SourceOutput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString source READ source)
    Q_PROPERTY(QString sourceProcessBinaryName READ sourceProcessBinaryName)
    Q_PROPERTY(int index READ index)
public:
    explicit SourceOutput(const QString& name = "",
                          int source = -1,
                          const QString& sourceProcessBinaryName = "",
                          int index = -1,
                          QObject *parent = nullptr);

    QString name() const;
    int source() const;
    QString sourceProcessBinaryName() const;
    int index() const;

    QString m_name;
    QString m_sourceProcessBinaryName;
    int m_source;
    int m_index;

};

#endif // SOURCEOUTPUT_H
