#ifndef FAKELIBQMLINTERFACE_H
#define FAKELIBQMLINTERFACE_H

#include "sink.h"
#include "source.h"
#include "sourceoutput.h"
#include "sinkinput.h"

#include <FakeLib.h>
#include <FakeMicWavPlayerLib.h>
#include <QQmlObjectListModel.h>

#include <QObject>
#include <QQmlListProperty>
#include <QThread>
#include <QDebug>
#include <QList>

#include <vector>

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
	Q_PROPERTY(QQmlObjectListModelBase* sinks READ sinks NOTIFY sinksChanged)
	Q_PROPERTY(QQmlObjectListModelBase* sources READ sources NOTIFY sourcesChanged)
	Q_PROPERTY(QQmlObjectListModelBase* sourceOutputs READ sourceOutputs NOTIFY sourceOutputsChanged)
	Q_PROPERTY(QQmlObjectListModelBase* sinkInputs READ sinkInputs NOTIFY sinkInputsChanged)
public:
    explicit FakeLibQmlInterface(QObject *parent = nullptr);
	~FakeLibQmlInterface();

    Q_INVOKABLE bool updateSinksList();
    Q_INVOKABLE bool updateSourcesList();
    Q_INVOKABLE bool updateSourceOutputsList();
    Q_INVOKABLE bool updateSinkInputsList();
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

	QQmlObjectListModel<Sink>* sinks();
	QQmlObjectListModel<Source>* sources();
	QQmlObjectListModel<SourceOutput>* sourceOutputs();
	QQmlObjectListModel<SinkInput>* sinkInputs();

public slots:
	void setNotRunning();

signals:
	void runningChanged(bool);

	void sinksChanged();
	void sourcesChanged();
	void sourceOutputsChanged();
	void sinkInputsChanged();

private:
    QQmlObjectListModel<Sink> m_sinks;
    QQmlObjectListModel<Source> m_sources;
    QQmlObjectListModel<SourceOutput> m_sourceOutputs;
    QQmlObjectListModel<SinkInput> m_sinkInputs;

    FakePlayerThread* fakePlayerThread;
	bool m_running;

    void makeFakePlayerThread();
};

#endif // FAKELIBQMLINTERFACE_H
