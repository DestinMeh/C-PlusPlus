#include <QGuiApplication>        // <--- This is the one you're likely missing
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringLiteral>
#include "AudioBridge.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    // 1. Create the engine that runs the UI
    QQmlApplicationEngine engine;

    // 2. Instantiate your C++ AudioBridge
    AudioBridge audioBridge;

    // 3. Make the AudioBridge available to QML (nickname: myAudioPlayer)
    engine.rootContext()->setContextProperty("myAudioPlayer", &audioBridge);

    // 4. Load the UI from the Resource System (qrc)
    // Note: The path depends on your CMake project name. 
    // Usually it is "qrc:/qt/qml/[ProjectName]/main.qml" or just "qrc:/main.qml"
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    // This connection ensures the app closes if the QML fails to load
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}