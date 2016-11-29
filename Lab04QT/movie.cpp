#include "movie.h"
#include <QFile>
#include <QTextStream>
#include <random>
#include <sys/time.h>
#include <QProcess>

Movie::Movie(QObject *parent) :
    QObject(parent)
{
    setup_seed();
    readFile();
}

// TODO: Add in validation and a batch file to grab the system time
void Movie::okBTN(QString msg)
{
    movie_data *temp;
    if(msg.toInt() % 2 == 1){
        // Odd
        temp = &movie_odd[getRandom(0, 4)];
    }
    else{
        // Even
        temp = &movie_even[getRandom(0, 4)];
    }
    runScript();
    emit surpriseMovie(temp->title);
    emit watchedTime(temp->lastWatched);
    emit currentTime(currTme);
    emit price(temp->rentalPrice);
    temp->lastWatched = currTme;
    qDebug() << temp->lastWatched;
    qDebug() << "OK Btn was pressed";
}

// TODO: Need to fix this to properly read in the input.txt
void Movie::readFile()
{
    QFile file(":/input.txt");

    if(!file.exists())
    {
        qDebug() << "Unable to find file!";
        return;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file!";
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
                    movie_even[even_j].title = title;
                    movie_even[even_j].rentalPrice = QString().sprintf("$%.2f", price);
                    even_j++;
            }
            else
            {

                movie_odd[odd_j].title = title;
                movie_odd[odd_j].rentalPrice = QString().sprintf("$%.2f", price);;
                odd_j++;
            }

            i++;
        }

    }

}


int Movie::getRandom(int rangeLow, int rangeHigh)
{
    double myRand = rand()/(1.0 + RAND_MAX);
    int range = rangeHigh - rangeLow +1;
    int myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
}

void Movie::setup_seed()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((unsigned int) time.tv_usec);
}
void Movie::runScript()
{
    system("chmod +x ~/OSLab/CMPSC441/Lab04QT/time.sh");
    system("~/OSLab/CMPSC441/Lab04QT/time.sh");
    QFile time("time.txt");
    if(!time.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Could not open file";
        return;
    }
    QTextStream in (&time);
    while(!in.atEnd())
    {
        currTme = in.readLine();
    }
    qDebug() << currTme;
    //qDebug() << process.readAllStandardError();
}
