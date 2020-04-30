#include "fakelibqmlinterface.h"


#include <QDebug>

FakeLibQmlInterface::FakeLibQmlInterface(QObject *parent) :
    QObject(parent), sinks_count(0), fakePlayerThread(nullptr)
{
    for (std::size_t i = 0; i < m_sinks.size(); ++i)
        m_sinks[i] = new Sink;
    for (std::size_t i = 0; i < m_sinks.size(); ++i)
        m_sourceOutputs[i] = new SourceOutput;
}

void FakeLibQmlInterface::updateSinksList()
{
    sink_infos_t sinks[16];
    if (get_sources_source_output_sinks_and_modules(NULL, NULL, sinks, NULL) < 0) {
        qDebug() << "Error, Failed to fetch sinks.";
        return;
    }
    sinks_count = 0;
    for (int ctr = 0; ctr < 16; ++ctr) {
        if (!sinks[ctr].initialized) {
                break;
        }
//        qDebug() << "=======[ Sink #" << ctr + 1 << " ]=======";
//        qDebug() << "Description: " << sinks[ctr].description;
//        qDebug() << "Name: " << sinks[ctr].name;
//        qDebug() << "Index: " << sinks[ctr].index;
//        qDebug();
        QString name(sinks[ctr].name);

        if (name == "TestSink" || name == "TestCombinedSink")
            continue;

        m_sinks[sinks_count]->m_name = sinks[ctr].name;
        m_sinks[sinks_count]->m_description = sinks[ctr].description;
        m_sinks[sinks_count]->m_index = sinks[ctr].index;
        ++sinks_count;
    }
}

void FakeLibQmlInterface::updateSourceOuputsList()
{
    source_output_infos_t source_outputs[16];
    if (get_sources_source_output_sinks_and_modules(NULL, source_outputs, NULL, NULL) < 0) {
        qDebug() << "Error, Failed to fetch source outputs.";
        return;
    }
    source_outputs_count = 0;
    for (int ctr = 0; ctr < 16; ++ctr) {
        if (!source_outputs[ctr].initialized) {
                break;
        }
//        qDebug() << "=======[ Source Output #" << ctr + 1 << " ]=======";
//        qDebug() << "Name: " << source_outputs[ctr].name;
//        qDebug() << "Source: " << source_outputs[ctr].source;
//        qDebug() << "Source Process Binary Name : " << source_outputs[ctr].source_process_binary;
//        qDebug() << "Index: " << source_outputs[ctr].index;
//        qDebug();

        if (strcmp("pavucontrol", source_outputs[ctr].source_process_binary) == 0)
            continue;

        m_sourceOutputs[source_outputs_count]->m_name = source_outputs[ctr].name;
        m_sourceOutputs[source_outputs_count]->m_source = source_outputs[ctr].source;
        m_sourceOutputs[source_outputs_count]->m_sourceProcessBinaryName = source_outputs[ctr].source_process_binary;
        m_sourceOutputs[source_outputs_count]->m_index = source_outputs[ctr].index;
        ++source_outputs_count;
    }
}

SourceOutput *FakeLibQmlInterface::sourceOutputAt(int index)
{
    return m_sourceOutputs.at(index);
}

int FakeLibQmlInterface::sourceOutputsCount() const
{
    return source_outputs_count;
}

Sink* FakeLibQmlInterface::sinkAt(int index)
{
    return m_sinks.at(index);
}

int FakeLibQmlInterface::sinkCount() const
{
    return sinks_count;
}

void FakeLibQmlInterface::fakePlay(const QString &wavFilePath, const QString &sinks, const QString &processBinaryName)
{
    qDebug() << "Fake play : " << processBinaryName;
    makeFakePlayerThread();
    fakePlayerThread->wavFilePath = wavFilePath;
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

