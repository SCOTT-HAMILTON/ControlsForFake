#ifndef FAKEPLAYERTHREAD_H
#define FAKEPLAYERTHREAD_H

#include <QThread>

enum AudioStreamInput { AUDIO_FILE, APPLICATION };

class FakePlayerThread : public QThread
{ 
	Q_OBJECT 
public:
    explicit FakePlayerThread(QObject* parent = nullptr);
    QString oggFilePath;
    QString applicationBinaryName;
	QString source;
    QString sinks;
    QString processBinaryName;
	AudioStreamInput streamInputMode;
	bool error;

protected:
    void run() override;

public slots:
    void stop();

signals:
    void processFinished();

private:
    std::atomic_bool canRun;

};

#endif //FAKEPLAYERTHREAD_H
