#include <stdio.h>
#include <stdlib.h>


//Struct deifinitions
struct Workspace{
   char pos[4][4];
};

struct Robot{
   int pos_x;
   int pos_y;
};

struct GoldBar{
   int pos_x;
   int pos_y;
}

struct Bomb{
   int pos_x;
   int pos_y;
}


// API?
void API();
void API()
{
	
}

// Create World
void createWorld();
void createWorld()
{
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
    {
      var[i][j] ='-';
    }
  }
}

// create random
int getRandom(int low, int high){
  double myRand = rand()/(1.0 + RAND_MAX);
  int range = rangeHigh - rangeLow +1;
  int myRand_scaled = (myRand * range)  rangeLow;
  return myRand_scaled;
}

// Rand Positions
void randPos();
void randPos()
{
  int i, result, low = 0, high = 15, place[4]= {-1};
  bool similar;

  struc timeval time;
  gettimeofday(&time, NULL);
  srandom((unsigned int) time.tv_usec);

  for(i = 0; i < 4; i++)
  {
    do{
      similar = fasle;
      result = getRandom(low, high);
      for(j = 0; j < 4; j++){
        if(result == place[j]){
          similar = true;
        }
      }
    }while(similar);
    place[i] = result;
  }

  // add the positions to the workplace
}


	// Run4Gold
	// Check for available gold
	// Check next Square
	// Check for bomb
	// jump next Square
	// Check Gold
	// Get Gold

int main()
{

	return 0;
}
