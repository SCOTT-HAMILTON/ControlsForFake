#include "SubscribeAndListenThread.h"

#include <QDebug>
#include <functional>
#include <thread>
#include <chrono>

FakeLib SubscribeAndListenThread::fakeLib;

SubscribeAndListenThread::SubscribeAndListenThread(QObject* parent) : 
	QThread(parent),
	callback(std::bind(&SubscribeAndListenThread::eventCallback, this, std::placeholders::_1))
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
			, callback);
	canRun = true;
	while (canRun) {
		fakeLib.iterate_subscribtion_loop();
		std::this_thread::sleep_for (std::chrono::milliseconds(500));
	}
	fakeLib.clean_subscribtion_loop();
}

void SubscribeAndListenThread::eventCallback(pa_subscription_event_type_t event) {
	emit newEvent(event);
}

void SubscribeAndListenThread::stop() {
	canRun = false;
}
