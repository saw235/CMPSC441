#include <stdio.h>
#include <stdlib.h>

int getRandom(int rangeLow, int rangeHigh) {
    double myRand = rand()/(1.0 + RAND_MAX);
    int range = rangeHigh - rangeLow + 1;
    int myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
}

int main()
{   int i,result, rangeLow=20, rangeHigh=50;

    struct timeval time;
    gettimeofday(&time, NULL);

    //srand for windows instead
    srandom((unsigned int) time.tv_usec);
    

    for(i=0;i<10;i++){
        result= getRandom(rangeLow,rangeHigh);
        printf("result = %d\n", result);
    }
    return 0;
}

