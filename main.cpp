#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QTranslator>
#include <QtQmlTricksPlugin_SmartDataModels.h>

#include "fakelibqmlinterface.h"


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QT_QUICK_CONTROLS_STYLE", "material");
    QApplication app(argc, argv);


    QTranslator translator;
    // look up ://translations/Lightning-en_US.qm
    QLocale locale;//(QLocale::English, QLocale::UnitedStates);
    if (translator.load("://translations/ControlsForFake_"+locale.name()+".qm"))
        app.installTranslator(&translator);

    QQmlApplicationEngine engine;
    qmlRegisterType<Sink>("org.controlfake.sink", 1, 0, "Sink");
    qmlRegisterType<Source>("org.controlfake.source", 1, 0, "Source");
    qmlRegisterType<SourceOutput>("org.controlfake.sourceOutput", 1, 0, "SourceOutput");
    qmlRegisterType<SinkInput>("org.controlfake.sinkInput", 1, 0, "SinkInput");
	registerQtQmlTricksSmartDataModel(&engine);

    auto fakelibQmlInterface = new FakeLibQmlInterface(&engine);
	fakelibQmlInterface->updateModels();
	fakelibQmlInterface->startPulseAudioSubscribtionListener();
    engine.rootContext()->setContextProperty("fakelibQmlInterface", fakelibQmlInterface);
	QQmlEngine::setObjectOwnership(fakelibQmlInterface, QQmlEngine::CppOwnership);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

	QObject::connect(fakelibQmlInterface, SIGNAL(sinksChanged()),
			engine.rootObjects()[0], SLOT(onSinksChanged()));
	QObject::connect(fakelibQmlInterface, SIGNAL(sourcesChanged()),
			engine.rootObjects()[0], SLOT(onSourcesChanged()));
	QObject::connect(fakelibQmlInterface, SIGNAL(sourceOutputsChanged()),
			engine.rootObjects()[0], SLOT(onSourceOutputsChanged()));
	QObject::connect(fakelibQmlInterface, SIGNAL(sinkInputsChanged()),
			engine.rootObjects()[0], SLOT(onSinkInputsChanged()));

    return app.exec();
}
