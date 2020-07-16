#ifndef SOURCEOUTPUT_H
#define SOURCEOUTPUT_H

#include <QObject>

class SourceOutput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString source READ source)
    Q_PROPERTY(QString processBinaryName READ processBinaryName)
    Q_PROPERTY(int pa_index READ index)
public:
    explicit SourceOutput(const QString& name = "",
                          int source = -1,
                          const QString& processBinaryName = "",
                          int index = -1,
                          QObject *parent = nullptr);

    QString name() const;
    int source() const;
    QString processBinaryName() const;
    int index() const;

    QString m_name;
    QString m_processBinaryName;
    int m_source;
    int m_index;

};

#endif // SOURCEOUTPUT_H
