#ifndef FAKELIBQMLINTERFACE_H
#define FAKELIBQMLINTERFACE_H

#include "sink.h"
#include "source.h"
#include "sourceoutput.h"
#include "sinkinput.h"
#include "fakeplayerthread.h"
#include "SubscribeAndListenThread.h"

#include <FakeLib/FakeLib.h>
#include <pulse/subscribe.h>
#include <QQmlObjectListModel.h>

#include <QObject>
#include <QQmlListProperty>
#include <QDebug>
#include <QList>

#include <vector>

Q_DECLARE_METATYPE(pa_subscription_event_type_t)

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
	Q_INVOKABLE void startPulseAudioSubscribtionListener();
	Q_INVOKABLE void clean();
	bool running() const;

	QQmlObjectListModel<Sink>* sinks();
	QQmlObjectListModel<Source>* sources();
	QQmlObjectListModel<SourceOutput>* sourceOutputs();
	QQmlObjectListModel<SinkInput>* sinkInputs();

public slots:
	void setNotRunning();
	void onNewPulseAudioEvent(pa_subscription_event_type_t event);
	void updateModels();

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
	SubscribeAndListenThread* pulseaudioSubscribeThread;
	bool m_running;

    void makeFakePlayerThread();
    void makePulseaudioSubscribeThread();
	static FakeLib fakeLib;
};

#endif // FAKELIBQMLINTERFACE_H
