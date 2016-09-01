#include <stdio.h>
#include <stdlib.h>


//Struct deifinitions
struct Robot{
   int pos_x;
   int pos_y;
};

struct GoldBar{
   int pos_x;
   int pos_y;
   bool available;
}

struct Bomb{
   int pos_x;
   int pos_y;
}

struct Workspace{
   char pos[4][4];
   int  n_gold
   struct Robot wall_e;
   struct GoldBar gb1;
   struct GoldBar gb2;

   struct Bomb bmb;
	
};


// API
void API();
void API()
{
	
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
  randPos(map);
}

// create random
int getRandom(int low, int high){
  double myRand = rand()/(1.0 + RAND_MAX);
  int range = rangeHigh - rangeLow +1;
  int myRand_scaled = (myRand * range)  rangeLow;
  return myRand_scaled;
}

// Rand Positions
void randPos(struct Workspace* map)
{
  int x, y, result, low = 0, high = 15, place[4]= {-1};
  bool similar;

  struc timeval time;
  gettimeofday(&time, NULL);
  srandom((unsigned int) time.tv_usec);

  for(x = 0; x < 4; y++)
  {
    do{
      similar = fasle;
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
  map->Wall_e.pos_x = x;
  map->Wall_e.pos_y = y;
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


// Run4Gold




// Update map
void UpdateWorkspace(struct Workspace* map)
{
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
    {
      map->pos[i][j] ='-';
    }
  }

  //update gold position on map
  if (map->gb1.available) { map->pos[map->gb1.pos_x][map->gb1.pos_y] = 'R';}
  if (map->gb2.available) { map->pos[map->gb2.pos_x][map->gb2.pos_y] = 'R';}
  //update wall-e's position on the map
  map->pos[map->wall_e.pos_x][map->wall_e.pos_y] = 'R';

 
}


//check if there is gold available in the map
bool MapHasGold(struct Workspace* map)
{
   return (map->n_gold != 0)?1:false;
}

// Check next Square
// Check for bomb
// jump next Square


// Check if Robot Stumble Upon Gold
bool hasGold(struct Workspace* map)
{
   int pos_x = map->wall_e.pos_x;
   int pos_y = map->wall_e.pos_y;

   if (map->pos[pos_x][pos_y] == 'G')
 	{
	   return 1;
	}
   else  { return 0;}  

}


// Get Gold



int main()
{

	return 0;
}
