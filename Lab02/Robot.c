#include <stdio.h>
#include <stdlib.h>


//Struct deifinitions
struct Workspace{
   char pos[4][4];
   int  n_gold;
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
// Rand Positions



// Run4Gold



// Check for available gold
bool hasGold(struct Workspace* map)
{
 
  return (map->n_gold != 0)?1:false;
}


// Check next Square
// Check for bomb
// jump next Square
// Check Gold
// Get Gold

int main()
{

	return 0;
}
