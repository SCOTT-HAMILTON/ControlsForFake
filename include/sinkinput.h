#ifndef SINKINPUT_H
#define SINKINPUT_H

#include <QObject>

class SinkInput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString sink READ sink)
    Q_PROPERTY(QString processBinaryName READ processBinaryName)
    Q_PROPERTY(int index READ index)
public:
    explicit SinkInput(const QString& name = "",
                          int sink = -1,
                          const QString& processBinaryName = "",
                          int index = -1,
                          QObject *parent = nullptr);

    QString name() const;
    int sink() const;
    QString processBinaryName() const;
    int index() const;

    QString m_name;
    QString m_processBinaryName;
    int m_sink;
    int m_index;

};

#endif // SINKINPUT_H
