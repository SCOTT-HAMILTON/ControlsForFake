#include "fakelibqmlinterface.h"

#include <QDebug>
#include <iterator>
#include <QQmlEngine>
#include <FakeLibUtils.hpp>
#include <FakeMicWavPlayerLib.h>

FakeLib FakeLibQmlInterface::fakeLib;

FakeLibQmlInterface::FakeLibQmlInterface(QObject *parent) :
    QObject(parent),
	fakePlayerThread(nullptr), pulseaudioSubscribeThread(nullptr),
	m_running(false)
{
	qRegisterMetaType<pa_subscription_event_type_t>();
}
FakeLibQmlInterface::~FakeLibQmlInterface()
{
	if (fakePlayerThread != nullptr) {
		qDebug() << "Stoping...";
		fakePlayerThread->stop();
		qDebug() << "Waiting...";
		fakePlayerThread->wait();
		qDebug() << "Cleaning...";
	}
	if (pulseaudioSubscribeThread != nullptr) {
		qDebug() << "Stoping...";
		pulseaudioSubscribeThread->stop();
		qDebug() << "Waiting...";
		pulseaudioSubscribeThread->wait();
		qDebug() << "Cleaning...";
	}
	FakeMicWavPlayer::clean();
	qDebug() << "Done";
}

bool FakeLibQmlInterface::updateSinksList()
{
	auto result = fakeLib
		.clear_commands()
		.get_sink_list()
		.run_commands();
	info_list<sink_infos_t> sink_list;
	try {
		sink_list = FakeLibUtils::extract<info_list<sink_infos_t>>(result);
	} catch (ObjectNotFoundError&) {
		std::cerr << "[error] FakeLibQMLInterface, couldn't fetch sink list, cancelling";
		return false;
	}
	if (m_sinks.size() > 0) {
		for (auto c : m_sinks) {
			if (c)
				c->deleteLater();
		}
		m_sinks.clear();
	}
    for (int ctr = 0; ctr < info_list_size; ++ctr) {
		// We assume that as soon as we hit initialized sinks, we've reached the 
		// end of the initialized sinks
        if (!sink_list[ctr].initialized) {
                break;
        }
        if (sink_list[ctr].name == fakeCombinedSinkName)
            continue;
		auto new_sink = new Sink(
			sink_list[ctr].name.c_str(),
			sink_list[ctr].description.c_str(),
			sink_list[ctr].index
		);
		m_sinks.append(new_sink);
    }
	if (m_sinks.size() == 0) {
		auto new_sink = new Sink(
			"",
			tr("No output device available"),
			-1
		);
		m_sinks.append(new_sink);
	}
	emit sinksChanged();
	return true;
}

bool FakeLibQmlInterface::updateSourcesList()
{
	auto result = fakeLib
		.clear_commands()
		.get_source_list()
		.run_commands();
	info_list<source_infos_t> source_list;
	try {
		source_list = FakeLibUtils::extract<info_list<source_infos_t>>(result);
	} catch (ObjectNotFoundError&) {
		std::cerr << "[error] FakeLibQMLInterface, couldn't fetch source list, cancelling";
		return false;
	}
	if (m_sources.size() > 0) {
		for (auto c : m_sources) {
			if (c)
				c->deleteLater();
		}
		m_sources.clear();
	}
    for (int ctr = 0; ctr < info_list_size; ++ctr) {
		// We assume that as soon as we hit initialized sources, we've reached the 
		// end of the initialized sources
        if (!source_list[ctr].initialized) {
                break;
        }
		// Source Is a monitor (name ends with .monitor)
		if (auto& name = source_list[ctr].name; name.rfind(".monitor") == name.size()-8) {
			continue;
		}
		auto new_source = new Source(
			source_list[ctr].name.c_str(),
			source_list[ctr].description.c_str(),
			source_list[ctr].index
		);
		m_sources.append(new_source);
    }
	if (m_sources.size() == 0) {
		auto new_source = new Source (
			"",
			tr("No input device available"),
			-1
		);
		m_sources.append(new_source);
	}
	emit sourcesChanged();
	return true;
}

bool FakeLibQmlInterface::updateSourceOutputsList()
{
	auto result = fakeLib
		.clear_commands()
		.get_source_output_list()
		.run_commands();
	info_list<source_output_infos_t> source_output_list;
	try {
		source_output_list = FakeLibUtils::extract<info_list<source_output_infos_t>>(result);
	} catch (ObjectNotFoundError&) {
		std::cerr << "[error] FakeLibQMLInterface, couldn't fetch source output list, cancelling";
		return false;
	}
	if (m_sourceOutputs.size() > 0) {
		for (auto c : m_sourceOutputs) {
			if (c)
				c->deleteLater();
		}
		m_sourceOutputs.clear();
	}
    for (int ctr = 0; ctr < info_list_size; ++ctr) {
		// We assume that as soon as we hit initialized sinks, we've reached the 
		// end of the initialized sinks
        if (!source_output_list[ctr].initialized) {
                break;
        }
		auto new_sourceOutput = new SourceOutput (
			source_output_list[ctr].name.c_str(),
			source_output_list[ctr].source,
			source_output_list[ctr].process_binary.c_str(),
			source_output_list[ctr].index
		);
		m_sourceOutputs.append(new_sourceOutput);
    }
	if (m_sourceOutputs.size() == 0) {
		auto new_sourceOutput = new SourceOutput (
			"",
			0,
			tr("No application available"),
			-1
		);
		m_sourceOutputs.append(new_sourceOutput);
	}
	emit sourceOutputsChanged();
	return true;
}

bool FakeLibQmlInterface::updateSinkInputsList()
{
	auto result = fakeLib
		.clear_commands()
		.get_sink_input_list()
		.run_commands();
	info_list<sink_input_infos_t> sink_input_list;
	try {
		sink_input_list = FakeLibUtils::extract<info_list<sink_input_infos_t>>(result);
	} catch (ObjectNotFoundError&) {
		std::cerr << "[error] FakeLibQMLInterface, couldn't fetch sink input list, cancelling";
		return false;
	}
	if (m_sinkInputs.size() > 0) {
		for (auto c : m_sinkInputs) {
			if (c)
				c->deleteLater();
		}
		m_sinkInputs.clear();
	}
    for (int ctr = 0; ctr < info_list_size; ++ctr) {
		// We assume that as soon as we hit initialized sinks, we've reached the 
		// end of the initialized sinks
        if (!sink_input_list[ctr].initialized) {
                break;
        }
		auto new_sinkInput = new SinkInput (
			sink_input_list[ctr].name.c_str(),
			sink_input_list[ctr].sink,
			sink_input_list[ctr].process_binary.c_str(),
			sink_input_list[ctr].index
		);
		m_sinkInputs.append(new_sinkInput);
    }
	if (m_sinkInputs.size() == 0) {
		auto new_sinkInput = new SinkInput (
			"",
			0,
			tr("No application available"),
			-1
		);
		m_sinkInputs.append(new_sinkInput);
	}
	emit sinkInputsChanged();
	return true;
}

bool FakeLibQmlInterface::set_user_volume(double volume) {
	if (!m_running) {
		qDebug() << "[error] No audio is playing, can't set user volume";
		return true;
	}
	return FakeMicWavPlayer::set_user_volume(volume) != 0;
}

bool FakeLibQmlInterface::set_source_volume(double volume) {
	if (!m_running) {
		qDebug() << "[error] No audio is playing, can't set source volume";
		return true;
	}
	return FakeMicWavPlayer::set_source_volume(volume) != 0;
}

void FakeLibQmlInterface::playOggToApp(const QString &oggFilePath, 
									   const QString &source,
									   const QString &sinks,
									   const QString &processBinaryName)
{
    makeFakePlayerThread();
	fakePlayerThread->streamInputMode = AUDIO_FILE;
    fakePlayerThread->oggFilePath = oggFilePath;
    fakePlayerThread->source = source;
    fakePlayerThread->sinks = sinks;
    fakePlayerThread->processBinaryName = processBinaryName;
    fakePlayerThread->start();
	m_running = true;
	emit runningChanged(m_running);
}

void FakeLibQmlInterface::sendAppSoundToApp(const QString &applicationBinaryName, 
									   const QString &source,
									   const QString &sinks,
									   const QString &processBinaryName)
{
    makeFakePlayerThread();
	fakePlayerThread->streamInputMode = APPLICATION;
    fakePlayerThread->applicationBinaryName = applicationBinaryName;
    fakePlayerThread->source = source;
    fakePlayerThread->sinks = sinks;
    fakePlayerThread->processBinaryName = processBinaryName;
    fakePlayerThread->start();
	m_running = true;
	emit runningChanged(m_running);
}
void FakeLibQmlInterface::startPulseAudioSubscribtionListener() {
	makePulseaudioSubscribeThread();
	QObject::connect(pulseaudioSubscribeThread, &SubscribeAndListenThread::newEvent, this, &FakeLibQmlInterface::onNewPulseAudioEvent);
	pulseaudioSubscribeThread->start();
}

void FakeLibQmlInterface::clean() {
	m_running = false;
	emit runningChanged(m_running);
	FakeMicWavPlayer::clean();
}

bool FakeLibQmlInterface::running() const {
	return m_running;
}

void FakeLibQmlInterface::setNotRunning() {
	m_running = false;
	emit runningChanged(m_running);
}

void FakeLibQmlInterface::onNewPulseAudioEvent(pa_subscription_event_type_t event) {
	if ((event & PA_SUBSCRIPTION_EVENT_MODULE) ||
		(event & PA_SUBSCRIPTION_EVENT_NEW) ||
		(event & PA_SUBSCRIPTION_EVENT_REMOVE) ||
		(event & PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT) ||
		(event & PA_SUBSCRIPTION_EVENT_SINK_INPUT)
	   ) {
		updateModels();
	}
}

void FakeLibQmlInterface::updateModels() {
	updateSinksList();
	updateSourcesList();
	updateSourceOutputsList();
	updateSinkInputsList();
}

void FakeLibQmlInterface::makeFakePlayerThread()
{
    if (fakePlayerThread != nullptr){
        if (fakePlayerThread->isRunning() || !fakePlayerThread->isFinished()){
            qDebug() << "Error, can't make new thread, still running.";
            return;
        }
        fakePlayerThread->deleteLater();
    }
    fakePlayerThread = new FakePlayerThread(this);
}

void FakeLibQmlInterface::makePulseaudioSubscribeThread()
{
    if (pulseaudioSubscribeThread != nullptr){
        if (pulseaudioSubscribeThread->isRunning() || !pulseaudioSubscribeThread->isFinished()){
            qDebug() << "Error, can't make new thread, still running.";
            return;
        }
        pulseaudioSubscribeThread->deleteLater();
    }
    pulseaudioSubscribeThread = new SubscribeAndListenThread(this);
}

QQmlObjectListModel<Sink> *FakeLibQmlInterface::sinks() {
	return &m_sinks;
}
QQmlObjectListModel<Source>* FakeLibQmlInterface::sources() {
	return &m_sources;
}
QQmlObjectListModel<SourceOutput>* FakeLibQmlInterface::sourceOutputs() {
	return &m_sourceOutputs;
}
QQmlObjectListModel<SinkInput>* FakeLibQmlInterface::sinkInputs() {
	return &m_sinkInputs;
}
