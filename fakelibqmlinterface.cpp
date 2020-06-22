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

Sink* FakeLibQmlInterface::sinkAt(int index)
{
    return m_sinks.at(index);
}

int FakeLibQmlInterface::sinkCount() const
{
    return m_sinks.size();
}

void FakeLibQmlInterface::playOggToApp(const QString &oggFilePath, const QString &sinks, const QString &processBinaryName)
{
    qDebug() << "Fake play : " << processBinaryName;
    makeFakePlayerThread();
    fakePlayerThread->oggFilePath = oggFilePath;
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

