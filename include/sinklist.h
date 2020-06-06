#ifndef SINKLIST_H
#define SINKLIST_H

#include <QObject>

class SinkList : public QObject
{
    Q_OBJECT
public:
    explicit SinkList(QObject *parent = nullptr);

signals:

};

#endif // SINKLIST_H
