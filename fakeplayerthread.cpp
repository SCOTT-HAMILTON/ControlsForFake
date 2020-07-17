#include "fakeplayerthread.h"

#include <QDebug>
#include <FakeMicWavPlayerLib.h>

FakePlayerThread::FakePlayerThread(QObject* parent) :
	QThread(parent), canRun(true)
{
}

void FakePlayerThread::run() {
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

void FakePlayerThread::stop(){
	canRun = false;
}
