#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QStringList>
#include <QtDebug>
#include <QQuickView>

struct movie_odd
{
    QString movie_title;
    QString time_last_watched;
    double price;
} movie_odd_stack[20];


struct movie_even
{
    QString movie_title;
    QString time_last_watched;
    double price;
} movie_even_stack[20];

void readFile();

using namespace std;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *topLevel = engine.rootObjects().at(0);

    QQuickWindow * window = qobject_cast<QQuickWindow *> (topLevel);

    readFile();

    for (int i = 0; i < 5; i++)
    {
        cout << movie_odd_stack[i].movie_title.toUtf8().constData() << endl;
    }




    return app.exec();
}



void readFile()
{
    QFile file(":/input.txt");

    if(!file.exists())
    {
        //error message - "Unable to find file!"
        return;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //error message -  "Unable to open file!"
        return;
    }

    QTextStream in(&file);
    int i = 1;
    int odd_j = 0;
    int even_j = 0;
    while (!in.atEnd())
    {

        QString line = in.readLine();
        int pos_space = line.indexOf(' ');
        int pos_end = line.indexOf(';');
        if (pos_space != -1)
        {
            QString line2 = line.mid(pos_space+1);
            QStringList list = line2.split(';');
            QString title = list[0];
            double price = (int) list[1].toDouble();

            if (i % 2 == 0)
            {
                    movie_even_stack[even_j].movie_title = title;
                    movie_even_stack[even_j].price       = price;
                    even_j++;
            }
            else
            {

                movie_odd_stack[odd_j].movie_title = title;
                movie_odd_stack[odd_j].price       = price;
                odd_j++;
            }

            i++;
        }

    }

}
