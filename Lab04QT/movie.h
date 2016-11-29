#ifndef MOVIE_H
#define MOVIE_H

#include <QObject>
#include <QDebug>
#include <QProcess>

class Movie : public QObject
{
    Q_OBJECT
    public:
        explicit Movie(QObject *parent = 0);

    signals:
        void surpriseMovie(QVariant data);
        void watchedTime(QVariant data);
        void currentTime(QVariant data);
        void price(QVariant data);

    public slots:
        void okBTN(QString msg);

private:
        void readFile();
        int getRandom(int rangeLow, int rangeHigh);
        void setup_seed();
        void runScript();
        struct movie_data{
            QString title;
            QString lastWatched;
            QString rentalPrice;
        };
        movie_data movie_odd[20];
        movie_data movie_even[20];

        QString movieTitle;
        QString watchedTme;
        QString currTme;

};

#endif // MOVIE_H
