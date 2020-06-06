#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "fakelibqmlinterface.h"


int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QT_QUICK_CONTROLS_STYLE", "material");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;


    qmlRegisterType<Sink>("org.controlfake.sink", 1, 0, "Sink");
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
