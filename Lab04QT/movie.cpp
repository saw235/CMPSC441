#include "movie.h"
#include <QFile>
#include <QTextStream>
#include <random>
#include <sys/time.h>

Movie::Movie(QObject *parent) :
    QObject(parent)
{
    setup_seed();
    //readFile();
}

// TODO: Add in validation
void Movie::okBTN(QString msg)
{
    movie_data temp;
    if(msg.toInt() % 2 == 1){
        // Odd
        temp = movie_odd[getRandom(0, 4)];
    }
    else{
        // Even
        temp = movie_even[getRandom(0, 4)];
    }
    emit surpriseMovie(temp.title);
    emit watchedTime(temp.lastWatched);
    //emit currentTime();
    emit price(temp.rentalPrice);
}

// TODO: Need to fix this to properly read in the input.txt
void Movie::readFile()
{
    QFile file("input.txt");
    QString msg;
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Could not open file";
        return;
    }
    QString line;
    std::string quote[10];
    int i = 0;
    QTextStream in (&file);
    while(!in.atEnd())
    {
        line = in.readLine();
        quote[i] = line.toStdString();
        i++;
    }
    movieTitle = QString::fromStdString(quote[msg.toInt()-1]);
    qDebug() << movieTitle;
    file.close();
}


int Movie::getRandom(int rangeLow, int rangeHigh)
{
    double myRand = rand()/(1.0 + RAND_MAX);
    int range = rangeHigh = rangeLow +1;
    int myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
}

void Movie::setup_seed()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((unsigned int) time.tv_usec);
}
