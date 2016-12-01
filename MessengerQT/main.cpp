#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "guihandler.h"


int main(int argc, char *argv[])
{

    GuiHandler handler;

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *topLevel = engine.rootObjects().at(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);


    // Connect GUI to Handler
    QObject::connect(window, SIGNAL(connectToServer(QString, int)), &handler, SLOT(connectToServer(QString, int)));
    QObject::connect(window, SIGNAL(disconnect()), &handler, SLOT(disconnect()));
    QObject::connect(window, SIGNAL(sendMsg(QString)), &handler, SLOT(sendMsg(QString)));

    // Connect Handler to GUI
    QObject::connect(&handler, SIGNAL(newMsg(QString)), window, SLOT(incomingMsg(QString)));
    QObject::connect(&handler, SIGNAL(errorMsg(QString)), window, SLOT(error(QString)));




//    while (1) {

//        if (strcmp(rbuffer, "-1\n") == 0) {
//            b_exit = true;
//            break;
//        }
//    }


    return app.exec();
}