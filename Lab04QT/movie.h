#ifndef MOVIE_H
#define MOVIE_H

#include <QObject>
#include <QDebug>

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
        struct movie_data{
            QString title;
            QString lastWatched;
            QString rentalPrice;
        };
        movie_data movie_odd[10];
        movie_data movie_even[10];

        QString movieTitle;
        QString watchedTme;
        QString currTme;

};

#endif // MOVIE_H
