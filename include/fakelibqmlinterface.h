#ifndef FAKELIBQMLINTERFACE_H
#define FAKELIBQMLINTERFACE_H

#include "sink.h"
#include "sourceoutput.h"

extern "C" {
#include <FakeLib.h>
}

#include <QObject>
#include <QQmlListProperty>
#include <QThread>

#include <array>

class FakePlayerThread : public QThread
{
    Q_OBJECT
public:
    explicit FakePlayerThread(QObject* parent = nullptr) :
        QThread(parent), canRun(true)
    {
    }
    QString wavFilePath;
    QString sinks;
    QString processBinaryName;

protected:
    void run() override {
        FakeAndPlayWav(wavFilePath.toStdString().c_str(),
                       sinks.toStdString().c_str(),
                       processBinaryName.toStdString().c_str());
        emit processFinished();
    }

public slots:
    void stop(){
        canRun = false;
    }

signals:
    void processFinished();
    void serverStarted();

private:
    std::atomic_bool canRun;

};

class FakeLibQmlInterface : public QObject
{
    Q_OBJECT
public:
    explicit FakeLibQmlInterface(QObject *parent = nullptr);

    Q_INVOKABLE void updateSinksList();
    Q_INVOKABLE Sink* sinkAt(int index);
    Q_INVOKABLE int sinkCount() const;


    Q_INVOKABLE void updateSourceOuputsList();
    Q_INVOKABLE SourceOutput* sourceOutputAt(int index);
    Q_INVOKABLE int sourceOutputsCount() const;

    Q_INVOKABLE void fakePlay(const QString& wavFilePath,
                              const QString& sinks,
                              const QString& processBinaryName);

signals:

private:
    std::array<Sink*, 16> m_sinks;
    std::array<SourceOutput*, 16> m_sourceOutputs;
    int sinks_count;
    int source_outputs_count;

    FakePlayerThread* fakePlayerThread;

    void makeFakePlayerThread();
};

#endif // FAKELIBQMLINTERFACE_H
