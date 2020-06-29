#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>

class Source : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(int index READ index)

public:
    explicit Source(const QString& name = "",
                    const QString& description = "",
                    int index = -1,
                    QObject *parent = nullptr);
    QString name() const;
    QString description() const;
    int index() const;

    QString m_name;
    QString m_description;
    int m_index;
};

#endif // SOURCE_H
