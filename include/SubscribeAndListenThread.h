#ifndef SUBSRIBE_AND_LISTEN_THREAD_H
#define SUBSRIBE_AND_LISTEN_THREAD_H

#include <QThread>
#include <pulse/subscribe.h>
#include <FakeLib/FakeLib.h>

class SubscribeAndListenThread : public QThread
{ 
	Q_OBJECT 
public:
    explicit SubscribeAndListenThread(QObject* parent = nullptr);
	static SubscribeAndListenThread* singleton;

protected:
    void run() override;

public slots:
    void stop();

signals:
	void newEvent(pa_subscription_event_type_t event);
    void processFinished();

private:
    std::atomic_bool canRun;
	static FakeLib fakeLib;
};

#endif //SUBSRIBE_AND_LISTEN_THREAD_H
