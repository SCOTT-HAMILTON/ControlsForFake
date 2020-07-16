#ifndef SINK_H
#define SINK_H

#include <QObject>

class Sink : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(int pa_index READ index)

public:
    explicit Sink(const QString& name = "",
                  const QString& description = "",
                  int index = -1,
                  QObject *parent = nullptr);
    QString name() const;
    QString description() const;
    int index() const;

    QString m_name;
    QString m_description;
    int m_index;

private:

};

#endif // SINK_H
