#ifndef SUBSRIBE_AND_LISTEN_THREAD_H
#define SUBSRIBE_AND_LISTEN_THREAD_H

#include <QThread>
#include <pulse/subscribe.h>
#include <FakeLib.h>

class SubscribeAndListenThread : public QThread
{ 
	Q_OBJECT 
public:
    explicit SubscribeAndListenThread(QObject* parent = nullptr);

protected:
    void run() override;

public slots:
	void eventCallback(pa_subscription_event_type_t event);
    void stop();

signals:
	void newEvent(pa_subscription_event_type_t event);
    void processFinished();

private:
    std::atomic_bool canRun;
	static FakeLib fakeLib;
	std::function<void(pa_subscription_event_type_t event)> callback;

};

#endif //SUBSRIBE_AND_LISTEN_THREAD_H
