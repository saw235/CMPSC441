#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickView>
#include <QFile>
#include <QTextStream>
#include "movie.h"

int main(int argc, char *argv[])
{
    Movie myMovie;

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *topLevel = engine.rootObjects().at(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    // Connect from QML to C++
    QObject::connect(window, SIGNAL(okPressed(QString)),
                     &myMovie,SLOT(okBTN(QString)));

    // Connect from C++ to QML
    QObject::connect(&myMovie, SIGNAL(surpriseMovie(QVariant)),
                     window, SLOT(updateSurprise(QString)));
    QObject::connect(&myMovie, SIGNAL(watchedTime(QVariant)),
                     window, SLOT(updateWatched(QString)));
    QObject::connect(&myMovie, SIGNAL(currentTime(QVariant)),
                     window, SLOT(updateCurrent(QString)));
    QObject::connect(&myMovie, SIGNAL(price(QVariant)),
                     window, SLOT(updatePrice));

    return app.exec();
}
