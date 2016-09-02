#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Struct deifinitions
struct Robot{
   int pos_x;
   int pos_y;
};

struct GoldBar{
   int pos_x;
   int pos_y;
   bool available;
};

struct Bomb{
   int pos_x;
   int pos_y;
};

struct Workspace{
   char pos[4][4];
   int  n_gold;
   struct Robot wall_e;
   struct GoldBar gb1;
   struct GoldBar gb2;

   struct Bomb bmb;
	
};

int getRandom(int rangeLow, int rangeHigh) {
    double myRand = rand()/(1.0 + RAND_MAX);
    int range = rangeHigh - rangeLow + 1;
    int myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
}


// Rand Positions
void randPos(struct Workspace* map)
{
  int x, y, result, low = 0, high = 15, place[4]= {-1};
  bool similar;

  struct timeval time;
  gettimeofday(&time, NULL);
  srandom((unsigned int) time.tv_usec);

  for(x = 0; x < 4; y++)
  {
    do{
      similar = false;
      result = getRandom(low, high);
      for(y = 0; y < 4; y++){
        if(result == place[y]){
          similar = true;
        }
      }
    }while(similar);
    place[x] = result;
  }
// add the positions to the workplace
  x = place[0] / 4;
  y = place[0] % 4;
  map->wall_e.pos_x = x;
  map->wall_e.pos_y = y;
  map->pos[x][y] = 'R';

  x = place[1] / 4;
  y = place[1] % 4;
  map->bmb.pos_x = x;
  map->bmb.pos_y = y;
  map->pos[x][y] = 'B';

  x = place[2] / 4;
  y = place[2] % 4;
  map->gb1.pos_x = x;
  map->gb1.pos_y = y;
  map->pos[x][y] = 'G';

  x = place[3] / 4;
  y = place[3] % 4;
  map->gb2.pos_x = x;
  map->gb2.pos_y = y;
  map->pos[x][y] = 'G';
}

// Create World
void createWorld(struct Workspace* map)
{
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
    {
      map->pos[i][j] = '-';
    }
  }

  //Initialize everything to random position


  //randPos(map);  //Commented out to test cause its not working
}

// Print out the map
void printMap(struct Workspace* map)
{
  //char *string[256];
  int i;

  for(i = 0; i < 4; i++)
  {
    printf("------------------------\n| %c | %c | %c | %c |\n", map->pos[i][0], map->pos[i][1], map->pos[i][2], map->pos[i][3]);
  }
  printf("------------------------\n");
}

void API()
{
    struct Workspace map;

    //Initialize all struct variables
    createWorld(&map);	
    printMap(&map);
}


int main()
{
    //API();
	
    struct Workspace map;

    //Initialize all struct variables
    createWorld(&map);	
    printMap(&map);

    return 0;
}
