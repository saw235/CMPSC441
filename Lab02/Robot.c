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
  map->gb1.pos_x = x;
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
  // Initialize everything 
  map->wall_e.n_goldcollected = 0;
  map->n_gold = 2;
  map->gb1.available = true;
  map->gb2.available = true;
  // Place everything to random position
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

  map->pos[map->bmb.pos_x][map->bmb.pos_y] = 'B';

} // end updateWorkspace()

// MapHasGold()
// Description: Check if there is gold available in the map
bool MapHasGold(struct Workspace *map) {
<<<<<<< HEAD
  printf("Checking if there is gold on the map");
  return (map->n_gold != 0);
=======
  return (map->n_gold > 0);
>>>>>>> c79002ca0b55e40112bb96a591dd245f191edbf5
} // end MapHasGold()


// randomMove()
// Description: Get a random direction : Up, down, left, right , topright, topleft, bottomright, bottomleft
void randomMove(int* x, int* y)
{
  *x = getRandom(-1,1);
  *y = getRandom(-1,1);

<<<<<<< HEAD
// hasGold()
// Description: Check if Robot Stumble Upon Gold
bool hasGold(struct Workspace *map) {
  printf("Checking if on gold.\n"); // For debugging
  
  int pos_x = map->wall_e.pos_x;
  int pos_y = map->wall_e.pos_y;

  if (map->pos[pos_x][pos_y] == 'G') {
    printf("Found Gold");
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
  // Checking if on the first gold bar
  if(map->gb1.pos_x == pos_x && map->gb1.pos_y == pos_y){
    map->gb1.available = false;
  }
  else{
    map->gb2.available = false;
  }
  // Updating gold counter
  map->n_gold--;

} // end getGold()
=======
  printf("x = %d", *x);
  printf("y = %d", *y);
 
} // end randomMove()
>>>>>>> c79002ca0b55e40112bb96a591dd245f191edbf5

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


// checkNextSquare()
// Description: Check next square for possible movement
void checkNextSquare(struct Workspace *map) {

  //Possible move range for x and y are -1 to 1
  int x = 0, y = 0;
  

  //keep checking for possible move until we found one
  while(true){

	//generate a random direction
	randomMove(&x,&y);


	//if the move is valid then break out of loop
        if (isValidMove(map, x, y)) {break;}
        //if (test) {break;} //should be commented out when ifValidMove() is working

        }
	

  //if a valid move is found then to next square
   printf("Robot was at (%d,%d)\n", map->wall_e.pos_x, map->wall_e.pos_y);
   moveNext(map, x, y);

   printf("Robot is now at (%d,%d)\n", map->wall_e.pos_x, map->wall_e.pos_y);
  
} // end checkNextSquare()

// moveNext()
// Description: Move robot to next square
void moveNext(struct Workspace *map,const int x,const int y) {
	map->wall_e.pos_x = map->wall_e.pos_x + x;
	map->wall_e.pos_y = map->wall_e.pos_y + y;
} // end moveNext()

// hasGold()
// Description: Check if Robot Stumble Upon Gold
bool hasGold(struct Workspace *map) {
  printf("Checking if gold is encountered.\n"); // For debugging
  
  updateWorkspace(map);
  int pos_x = map->wall_e.pos_x;
  int pos_y = map->wall_e.pos_y;

  if ((map->gb1.pos_x == pos_x) && (map->gb1.pos_y == pos_y) && map->gb1.available){
    printf("Found gold!\n");
    return 1;
  }
  if ((map->gb2.pos_x == pos_x) && (map->gb2.pos_y == pos_y) && map->gb2.available){
    printf("Found gold!\n");
    return 1;
  }

  printf("Gold not found.\n");
  return 0; 
} // end hasGold()

// getGold()
// Description: Pick up the gold
void getGold(struct Workspace *map) {
  printf("Picking up gold\n"); // For debugging

  if((map->gb1.pos_x == map->wall_e.pos_x) && (map->gb1.pos_y == map->wall_e.pos_y) && map->gb1.available) { map->gb1.available = false; map->n_gold--; map->wall_e.n_goldcollected++;}

  if((map->gb1.pos_x == map->wall_e.pos_x) && (map->gb1.pos_y == map->wall_e.pos_y) && map->gb2.available) { map->gb1.available = false; map->n_gold--; map->wall_e.n_goldcollected++;} 
} // end getGold()

// Run4Gold()
// Description: Start the sequence for the Robot to search for gold
void Run4Gold(struct Workspace *map) {
  // Check if there is still golds in the map
  while (MapHasGold(map)) {
    // Update turn counter
    map->turnCount++;
    // debug message, should be commented out if not needed
    if (test) {printf("There is still gold in the map!\nChecking next square.\n");}

    // start sequence to examine next square and move    
    checkNextSquare(map);
    updateWorkspace(map);
    printMap(map);

    if (hasGold(map))
	{
	   getGold(map);
	}

    printMap(map);
    // created to break out of the loop when debugging
    if (test) { break;}
  }

  // end if no gold
  printf("There is no more gold in the map.\n");
  return;

} // end Run4Gold()

// printMap()
// Description: Output the map in a readable format to the console
void printMap(struct Workspace *map) {
  
  updateWorkspace(map);
  int i;

  for (i = 0; i < 4; i++) {
    printf("------------------------\n| %c | %c | %c | %c |\n", map->pos[0][i],
           map->pos[1][i], map->pos[2][i], map->pos[3][i]);
  }
  printf("------------------------\n");
} // end printMap()


//init2Fix()
void init2Fix(struct Workspace* map)
{
  map->n_gold = 2;

  map->wall_e.pos_x = 0;
  map->wall_e.pos_y = 0;
  map->wall_e.n_goldcollected = 0;

  map->gb1.pos_x = 3;
  map->gb1.pos_y = 0;
  map->gb1.available = true;

  map->gb2.pos_x = 2;
  map->gb2.pos_y = 3;
  map->gb2.available = true;

  map->bmb.pos_x = 0;
  map->bmb.pos_y = 3;

}
 




/**************************END FUNCTIONS******************************/

int main() {
  // API();

  struct Workspace map;

  // Initialize all struct variables
  // createWorld(&map);
  init2Fix(&map);
  printMap(&map);
  
  printf("Moving right by 1 unit\n");
  moveNext(&map, 1, 0);
  printMap(&map);

  printf("Moving right by 1 unit\n");
  moveNext(&map, 1, 0);
  printMap(&map);

  hasGold(&map);

  printf("Moving right by 1 unit\n");
  moveNext(&map, 1, 0);
  hasGold(&map);
  printMap(&map);

  printf("Moving down by 1 unit\n");
  moveNext(&map, 0, 1);
  hasGold(&map);
  printMap(&map);
  
  printf("Moving up by 1 unit\n");
  moveNext(&map, 0, -1);
  if (hasGold(&map))
  {getGold(&map);}
  printMap(&map);
  
  printf("Moving down by 1 unit\n");
  moveNext(&map, 0, 1);
  hasGold(&map);
  printMap(&map);
  
  printf("Moving up by 1 unit\n");
  moveNext(&map, 0, -1);
  if (hasGold(&map))
  {getGold(&map);}
  printMap(&map);

 // Run4Gold(&map);

  return 0;
}

