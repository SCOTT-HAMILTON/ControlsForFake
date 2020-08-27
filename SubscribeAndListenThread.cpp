#include "SubscribeAndListenThread.h"

#include <QDebug>
#include <functional>
#include <thread>
#include <chrono>

FakeLib SubscribeAndListenThread::fakeLib;
SubscribeAndListenThread* SubscribeAndListenThread::singleton = nullptr;

void eventCallback(pa_subscription_event_type_t event) {
	emit SubscribeAndListenThread::singleton->newEvent(event);
}

SubscribeAndListenThread::SubscribeAndListenThread(QObject* parent) : 
	QThread(parent)
{
}

void SubscribeAndListenThread::run() {
	fakeLib.init_subscribtion(
				static_cast<pa_subscription_mask_t>(
				PA_SUBSCRIPTION_MASK_SINK |
				PA_SUBSCRIPTION_MASK_SOURCE |
				PA_SUBSCRIPTION_MASK_SINK_INPUT |
				PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT |
				PA_SUBSCRIPTION_MASK_MODULE)
			, eventCallback);
	canRun = true;
	while (canRun) {
		fakeLib.iterate_subscribtion_loop();
		std::this_thread::sleep_for (std::chrono::milliseconds(500));
	}
	fakeLib.clean_subscribtion_loop();
}


void SubscribeAndListenThread::stop() {
	canRun = false;
}
