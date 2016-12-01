#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

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
    QObject::connect(window, SIGNAL(getConState()), &handler, SLOT(getConState()));

    // Connect Handler to GUI
    QObject::connect(&handler, SIGNAL(newMsg(QVariant)), window, SLOT(incomingMsg(QVariant)));
    QObject::connect(&handler, SIGNAL(errorMsg(QVariant)), window, SLOT(error(QVariant)));
    QObject::connect(&handler, SIGNAL(connectionState(QVariant)), window, SLOT(conState(QVariant)));




//    while (1) {

//        if (strcmp(rbuffer, "-1\n") == 0) {
//            b_exit = true;
//            break;
//        }
//    }


    return app.exec();
}
