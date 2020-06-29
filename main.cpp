#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>

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

    FakeLibQmlInterface fakelibQmlInterface;
    engine.rootContext()->setContextProperty("fakelibQmlInterface", &fakelibQmlInterface);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
