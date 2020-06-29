#ifndef FAKELIBQMLINTERFACE_H
#define FAKELIBQMLINTERFACE_H

#include "sink.h"
#include "source.h"
#include "sourceoutput.h"

#include <FakeLib.h>
#include <FakeMicWavPlayerLib.h>

#include <QObject>
#include <QQmlListProperty>
#include <QThread>
#include <QDebug>

#include <array>

class FakePlayerThread : public QThread
{ Q_OBJECT public:
    explicit FakePlayerThread(QObject* parent = nullptr) :
        QThread(parent), canRun(true)
    {
    }
    QString oggFilePath;
	QString source;
    QString sinks;
    QString processBinaryName;
	bool error;

protected:
    void run() override {
		error = false;
		qDebug() << "Thread : oggFilePath : " << oggFilePath;
		qDebug() << "Thread : source : " << source;
		qDebug() << "Thread : sinks : " << sinks;
		qDebug() << "Thread : processBinaryName : " << processBinaryName;
		if (FakeMicWavPlayer::init(oggFilePath.toStdString(),
								   source.toStdString(),
								   sinks.toStdString(),
								   processBinaryName.toStdString()) != 0) {
			error = true;
			emit processFinished();
			return;
		}
		canRun = true;
		while (canRun && FakeMicWavPlayer::playNonBlocking() == 0) {
		}
		FakeMicWavPlayer::cleanPlayer();
		FakeMicWavPlayer::clean();

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
	~FakeLibQmlInterface();

    Q_INVOKABLE bool updateSinksList();
    Q_INVOKABLE Sink* sinkAt(int index);
    Q_INVOKABLE int sinkCount() const;

    Q_INVOKABLE bool updateSourcesList();
    Q_INVOKABLE Source* sourceAt(int index);
    Q_INVOKABLE int sourceCount() const;

    Q_INVOKABLE bool updateSourceOuputsList();
    Q_INVOKABLE SourceOutput* sourceOutputAt(int index);
    Q_INVOKABLE int sourceOutputsCount() const;

	Q_INVOKABLE bool set_user_volume(double volume);
	Q_INVOKABLE bool set_source_volume(double volume);

    Q_INVOKABLE void playOggToApp(const QString& oggFilePath,
								  const QString& source,
								  const QString& sinks,
								  const QString& processBinaryName);

signals:

private:
    std::vector<Sink*> m_sinks;
    std::vector<Source*> m_sources;
    std::vector<SourceOutput*> m_sourceOutputs;

    FakePlayerThread* fakePlayerThread;

    void makeFakePlayerThread();
};

#endif // FAKELIBQMLINTERFACE_H
