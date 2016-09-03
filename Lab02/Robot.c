#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const bool test = true;  //set this to true when debugging to avoid infinite loop from unfinished code

// Struct deifinitions
struct Robot {
  int pos_x;
  int pos_y;

  int n_goldcollected;
};

struct GoldBar {
  int pos_x;
  int pos_y;
  bool available;
};

struct Bomb {
  int pos_x;
  int pos_y;
};

struct Workspace {
  char pos[4][4];
  int n_gold, turnCount;

  struct Robot wall_e;
  struct GoldBar gb1;
  struct GoldBar gb2;

  struct Bomb bmb;
};

// API
void API();
void API() {}

/**************************FUNCTIONS******************************/
// getRandom()
// Description: Takes in two ranges and return a random number between that.
int getRandom(int rangeLow, int rangeHigh) {
  double myRand = rand() / (1.0 + RAND_MAX);
  int range = rangeHigh - rangeLow + 1;
  int myRand_scaled = (myRand * range) + rangeLow;
  return myRand_scaled;
} // end getRandom()

// randPos()
void randPos(struct Workspace *map) {
  int x, y, result, low = 0, high = 15, place[4] = {-1};
  bool similar;

  struct timeval time;
  //gettimeofday(&time, NULL);
  srandom((unsigned int)time.tv_usec);

  for (x = 0; x < 4; x++) {
    do {
      similar = false;
      result = getRandom(low, high);
      for (y = 0; y < 4; y++) {
        if (result == place[y]) {
          similar = true;
        }
      }
    } while (similar);
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
  map->gb1.pos_y = y;
  map->pos[x][y] = 'G';

  x = place[3] / 4;
  y = place[3] % 4;
  map->gb2.pos_x = x;
  map->gb2.pos_y = y;
  map->pos[x][y] = 'G';

  // Set turn count to zero
  map->turnCount = 0;
  // Set the number of Gold
  map->n_gold = 2;
  // make gold available
  map->gb1.available = true;
  map->gb2.available = true;

} // end randPos()

// createWorld()
// Description: Initialize the workspace
void createWorld(struct Workspace *map) {

  printf("Populating map with blanks.\n"); // For debugging, remove if not needed

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      map->pos[i][j] = '-';
    }
  }

  printf("Initializing to random position");
  // Initialize everything to random position
   randPos(map);  //Commented out to test cause its not working

} // end createWorld()

// updateWorkspace()
// Description: Refreshes the map to reflect the current workspace
void updateWorkspace(struct Workspace *map) {

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      map->pos[i][j] = '-';
    }
  }

  // update gold position on map
  if (map->gb1.available) {
    map->pos[map->gb1.pos_x][map->gb1.pos_y] = 'G';
  }
  if (map->gb2.available) {
    map->pos[map->gb2.pos_x][map->gb2.pos_y] = 'G';
  }
  // update wall-e's position on the map
  map->pos[map->wall_e.pos_x][map->wall_e.pos_y] = 'R';

  // update the bomb position??
  map->pos[map->bmb.pos_x][map->bmb.pos_y] = 'B';

} // end updateWorkspace()

// printMap()
// Description: Output the map in a readable format to the console
void printMap(struct Workspace *map) {
  // char *string[256];
  int i;

  printf("\nTurn: %i\n", map->turnCount);
  for (i = 0; i < 4; i++) {
    printf("-----------------\n| %c | %c | %c | %c |\n", map->pos[i][0], map->pos[i][1], 
      map->pos[i][2], map->pos[i][3]);
  }
  printf("-----------------\n");
} // end printMap()

// MapHasGold()
// Description: Check if there is gold available in the map
bool MapHasGold(struct Workspace *map) {
  return (map->n_gold != 0);
} // end MapHasGold()


// randomMove()
// Description: Get a random direction : Up, down, left, right , topright, topleft, bottomright, bottomleft
void randomMove(int* x, int* y)
{
  *x = getRandom(-1,1);
  *y = getRandom(-1,1); 
} // end randomMove()

// hasGold()
// Description: Check if Robot Stumble Upon Gold
bool hasGold(struct Workspace *map) {
  printf("Checking if there is still gold present in the map.\n"); // For debugging
  
  int pos_x = map->wall_e.pos_x;
  int pos_y = map->wall_e.pos_y;

  if (map->pos[pos_x][pos_y] == 'G') {
    return true;
  } else {
    return false;
  }

} // end hasGold()

// getGold()
// Description: Pick up the gold
void getGold(struct Workspace *map) {
  int pos_x = map->wall_e.pos_x;
  int pos_y = map->wall_e.pos_y;

  printf("Picking up gold\n"); // For debugging
  if(map->gb1.pos_x == pos_x && map->gb1.pos_y == pos_y){
    map->gb1.available = false;
  }
  else{
    map->gb2.available = false;
  }
  map->n_gold--;

} // end getGold()

// hasBomb()
// Description: Check if the square has bomb in it
bool hasBomb(struct Workspace *map, int x, int y) {
  bool bmb = false;
  if (map->pos[x][y] == 'B'){
    bmb = true;
    printf("Bomb has been found, not moving.\n");
  }
  return bmb;
} // end hasBomb()

// isValidMove()
// Description: A move is valid if the Robot is not on a bomb and not out of range of the map
bool isValidMove(struct Workspace *map, int x, int y)
{
  bool valid;
  if(map->wall_e.pos_x + x > 4 || map->wall_e.pos_x + x < 0){valid = false;} // check if x is out of range
  else if(map->wall_e.pos_y + y > 4 || map->wall_e.pos_y + y < 0){valid = false;} // check if y is out of range
  else if(hasBomb(map, map->wall_e.pos_x + x, map->wall_e.pos_y + y)){valid = false;} // check if bomb is on the square
  else{
    valid = true;
    printf("Safe to move.\n");
  }
  return valid;
}

// moveNext()
// Description: Move robot to next square
void moveNext(struct Workspace *map, int x, int y) {
  map->wall_e.pos_x = x;
  map->wall_e.pos_y = y;

  if(hasGold(map)){getGold(map);}

} // end moveNext()

// Check next Squareompare/master...master?expand=1
// Description: Check next square for possible movement
void checkNextSquare(struct Workspace *map) {

  //Possible move range for x and y are -1 to 1
  int x , y = 0;
  

  //keep checking for possible move until we found one
  while(true){

	//generate a random direction
	randomMove(&x,&y);

	//if the move is valid then break out of loop
        if (isValidMove(map,x,y)) {break;}
        //if (test) {break;} //should be commented out when ifValidMove() is working
  }
	
  // Take the random move
  moveNext(map, x, y);

} // end checkNextSquare


// Run4Gold()
// Description: Start the sequence for the Robot to search for gold
void Run4Gold(struct Workspace *map) {
  // Check if there is still golds in the map
  while (MapHasGold(map)) {
    // Update turn counter
    map->turnCount++;
    // debug message, should be commented out if not needed
    if (test) {printf("There is still gold in the map!\nChecking next square.\n");}

    // start sequence to examine next square
    checkNextSquare(map);
    updateWorkspace(map);
    printMap(map);

    // created to break out of the loop when debugging
    if (test) { break;}
  }

  // end if no gold
  printf("There is no more gold in the map.\n");
  return;

} // end Run4Gold()


/**************************END FUNCTIONS******************************/

int main() {
  // API();

  struct Workspace map;

  // Initialize all struct variables
  createWorld(&map);
  printMap(&map);


  Run4Gold(&map);

  return 0;
}

