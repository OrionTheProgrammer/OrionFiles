#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QCoreApplication>

#include "backend/FileSystemModel.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName("Orion");
    QGuiApplication::setOrganizationName("OrionTheProgrammer");

    QQuickStyle::setStyle("Fusion");

    qmlRegisterType<FileSystemModel>("Orion.Backend", 1, 0, "FileSystemModel");

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    engine.loadFromModule("Orion", "Main");

    return app.exec();
}
