#include "fakelibqmlinterface.h"
#include "FakeLibUtils.hpp"

#include <QDebug>
#include <iterator>

FakeLibQmlInterface::FakeLibQmlInterface(QObject *parent) :
    QObject(parent), fakePlayerThread(nullptr)
{
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
	FakeMicWavPlayer::clean();
	qDebug() << "Done";
}

bool FakeLibQmlInterface::updateSinksList()
{
	FakeLib& fakeLib = FakeMicWavPlayer::fakeLib;
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
	qDeleteAll(std::begin(m_sinks), std::end(m_sinks));
	m_sinks.clear();
    for (int ctr = 0; ctr < info_list_size; ++ctr) {
		// We assume that as soon as we hit initialized sinks, we've reached the 
		// end of the initialized sinks
        if (!sink_list[ctr].initialized) {
                break;
        }
        if (sink_list[ctr].name == fakeCombinedSinkName)
            continue;
		m_sinks.emplace_back(new Sink());
		auto new_sink = m_sinks.back();
        new_sink->m_name = sink_list[ctr].name.c_str();
        new_sink->m_description = sink_list[ctr].description.c_str();
        new_sink->m_index = sink_list[ctr].index;
    }
	return true;
}

Sink* FakeLibQmlInterface::sinkAt(int index)
{
    return m_sinks.at(index);
}

int FakeLibQmlInterface::sinkCount() const
{
    return m_sinks.size();
}

bool FakeLibQmlInterface::updateSourcesList()
{
	FakeLib& fakeLib = FakeMicWavPlayer::fakeLib;
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
	qDeleteAll(std::begin(m_sources), std::end(m_sources));
	m_sources.clear();
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
		m_sources.emplace_back(new Source());
		auto new_source = m_sources.back();
        new_source->m_name = source_list[ctr].name.c_str();
        new_source->m_description = source_list[ctr].description.c_str();
        new_source->m_index = source_list[ctr].index;
    }
	return true;
}

Source* FakeLibQmlInterface::sourceAt(int index)
{
    return m_sources.at(index);
}

int FakeLibQmlInterface::sourceCount() const
{
    return m_sources.size();
}

bool FakeLibQmlInterface::updateSourceOuputsList()
{
	FakeLib& fakeLib = FakeMicWavPlayer::fakeLib;
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
	qDeleteAll(std::begin(m_sourceOutputs), std::end(m_sourceOutputs));
	m_sourceOutputs.clear();
    for (int ctr = 0; ctr < info_list_size; ++ctr) {
		// We assume that as soon as we hit initialized sinks, we've reached the 
		// end of the initialized sinks
        if (!source_output_list[ctr].initialized) {
                break;
        }
		m_sourceOutputs.emplace_back(new SourceOutput());
		auto new_sourceOutput = m_sourceOutputs.back();
        new_sourceOutput->m_name = source_output_list[ctr].name.c_str();
        new_sourceOutput->m_sourceProcessBinaryName = source_output_list[ctr].source_process_binary.c_str();
        new_sourceOutput->m_source = source_output_list[ctr].source;
        new_sourceOutput->m_index = source_output_list[ctr].index;
    }
	return true;
}

SourceOutput *FakeLibQmlInterface::sourceOutputAt(int index)
{
    return m_sourceOutputs.at(index);
}

int FakeLibQmlInterface::sourceOutputsCount() const
{
    return m_sourceOutputs.size();
}
bool FakeLibQmlInterface::set_user_volume(double volume) {
	if (fakePlayerThread != nullptr && !fakePlayerThread->isRunning()) {
		qDebug() << "[error] No audio is playing, can't set user volume";
		return true;
	}
	return FakeMicWavPlayer::set_user_volume(volume) != 0;
}

bool FakeLibQmlInterface::set_source_volume(double volume) {
	if (fakePlayerThread && !fakePlayerThread->isRunning()) {
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
    fakePlayerThread->oggFilePath = oggFilePath;
    fakePlayerThread->source = source;
    fakePlayerThread->sinks = sinks;
    fakePlayerThread->processBinaryName = processBinaryName;
    fakePlayerThread->start();
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

