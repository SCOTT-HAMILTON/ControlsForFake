#ifndef FAKELIBQMLINTERFACE_H
#define FAKELIBQMLINTERFACE_H

#include "sink.h"
#include "source.h"
#include "sourceoutput.h"
#include "sinkinput.h"

#include <FakeLib.h>
#include <FakeMicWavPlayerLib.h>

#include <QObject>
#include <QQmlListProperty>
#include <QThread>
#include <QDebug>

#include <array>

enum AudioStreamInput { AUDIO_FILE, APPLICATION };
class FakePlayerThread : public QThread
{ 
	Q_OBJECT 
public:
    explicit FakePlayerThread(QObject* parent = nullptr) :
        QThread(parent), canRun(true)
    {
    }
    QString oggFilePath;
    QString applicationBinaryName;
	QString source;
    QString sinks;
    QString processBinaryName;
	AudioStreamInput streamInputMode;
	bool error;

protected:
    void run() override {
		error = false;
		qDebug() << "Thread : source : " << source;
		qDebug() << "Thread : sinks : " << sinks;
		qDebug() << "Thread : processBinaryName : " << processBinaryName;

		switch  (streamInputMode) {
			case AUDIO_FILE: {
				qDebug() << "Thread : oggFilePath : " << oggFilePath;
				if (FakeMicWavPlayer::initWithAudioFile(oggFilePath.toStdString(),
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
				break;
			}
			case APPLICATION: {
				qDebug() << "Thread : Application : " << applicationBinaryName;
				if (FakeMicWavPlayer::initWithSinkInput(applicationBinaryName.toStdString(),
										   source.toStdString(),
										   sinks.toStdString(),
										   processBinaryName.toStdString()) != 0) {
					error = true;
					emit processFinished();
					return;
				}
			}
		}
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
	Q_PROPERTY(bool running	READ running NOTIFY runningChanged)
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

    Q_INVOKABLE bool updateSinkInputsList();
    Q_INVOKABLE SinkInput* sinkInputAt(int index);
    Q_INVOKABLE int sinkInputsCount() const;

	Q_INVOKABLE bool set_user_volume(double volume);
	Q_INVOKABLE bool set_source_volume(double volume);

    Q_INVOKABLE void playOggToApp(const QString& oggFilePath,
								  const QString& source,
								  const QString& sinks,
								  const QString& processBinaryName);
    Q_INVOKABLE void sendAppSoundToApp(const QString& applicationBinaryName,
								  const QString& source,
								  const QString& sinks,
								  const QString& processBinaryName);
	Q_INVOKABLE void clean();

	bool running() const;

public slots:
	void setNotRunning();

signals:
	void runningChanged(bool);

private:
    std::vector<Sink*> m_sinks;
    std::vector<Source*> m_sources;
    std::vector<SourceOutput*> m_sourceOutputs;
    std::vector<SinkInput*> m_sinkInputs;

    FakePlayerThread* fakePlayerThread;
	bool m_running;

    void makeFakePlayerThread();
};

#endif // FAKELIBQMLINTERFACE_H
