#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const bool test = false; // set this to true when debugging to avoid infinite
                        // loop from unfinished code

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

// Forward Declaration
int getRandom(int rangeLow, int rangeHigh);
void randPos(struct Workspace *map);
void createWorld(struct Workspace *map);
void updateWorkspace(struct Workspace *map);
bool MapHasGold(struct Workspace *map);
void randomMove(int *x, int *y);
bool hasBomb(struct Workspace *map, int x, int y);
bool isValidMove(struct Workspace *map, int x, int y);
void checkNextSquare(struct Workspace *map);
void moveNext(struct Workspace *map, const int x, const int y);
bool hasGold(struct Workspace *map);
void getGold(struct Workspace *map);
void Run4Gold(struct Workspace *map);
void printMap(struct Workspace *map);
void init(struct Workspace *map);
void robotAPI(struct Workspace *map);

// end Forward Declaration

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
  // gettimeofday(&time, NULL);
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

} // end randPos()

// createWorld()
// Description: Initialize the workspace
void createWorld(struct Workspace *map) {

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      map->pos[i][j] = '-';
    }
  }

} // end createWorld()

// updateWorkspace()
// Description: Refreshes the map to reflect the current workspace
void updateWorkspace(struct Workspace *map) {

  // use createWorld() to create fill with blanks
  createWorld(map);

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
  printf("Map has %d gold bar(s).\n", map->n_gold);
  return (map->n_gold > 0);
} // end MapHasGold()

// randomMove()
// Description: Get a random direction : Up, down, left, right , topright,
// topleft, bottomright, bottomleft
void randomMove(int *x, int *y) {
  // Randomize a direction
  do {
    *x = getRandom(0, 2) - 1;
    *y = getRandom(0, 2) - 1;
  } while ((*x == 0) && (*y == 0)); // make sure it is not (0,0)

  // printf("move_x = %d\n", *x);
  // printf("move_y = %d\n", *y);

} // end randomMove()

// Description: Check if the square has bomb in it
bool hasBomb(struct Workspace *map, int x, int y) {
  bool bmb = false;
  if (map->pos[x][y] == 'B') {
    bmb = true;
    printf("Bomb has been found, not moving.\n");
  }
  return bmb;
} // end hasBomb()

// isValidMove()
// Description: A move is valid if the Robot is not on a bomb and not out of
// range of the map
bool isValidMove(struct Workspace *map, int x, int y) {
  bool valid;
  if ((map->wall_e.pos_x + x >= 4) || (map->wall_e.pos_x + x) < 0) {
    valid = false;
  } // check if x is out of range
  else if ((map->wall_e.pos_y + y >= 4) || (map->wall_e.pos_y + y < 0)) {
    valid = false;
  } // check if y is out of range
  else if (hasBomb(map, map->wall_e.pos_x + x, map->wall_e.pos_y + y)) {
    valid = false;
  } // check if bomb is on the square
  else {
    valid = true;
    printf("Safe to move.\n");
  }

  // printf("\n Move is %d \n", valid);
  return valid;
}

// checkNextSquare()
// Description: Check next square for possible movement
void checkNextSquare(struct Workspace *map) {

  // Possible move range for x and y are -1 to 1
  int x, y;

  // keep checking for possible move until we found one
  while (true) {

    x = y = 0;
    // generate a random direction
    randomMove(&x, &y);

    printf("Checking square (%d,%d).\n", map->wall_e.pos_x + x + 1,
           map->wall_e.pos_y + y + 1);
    // if the move is valid then break out of loop
    if (isValidMove(map, x, y)) {
      break;
    }
    // if (test) {break;} //should be commented out when ifValidMove() is
    // working
  }

  // if a valid move is found then to next square

  moveNext(map, x, y);
  printf("Robot is now at (%d,%d)\n", map->wall_e.pos_x + 1,
         map->wall_e.pos_y + 1);

} // end checkNextSquare()

// moveNext()
// Description: Move robot to next square
void moveNext(struct Workspace *map, const int x, const int y) {
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

  if ((map->gb1.pos_x == pos_x) && (map->gb1.pos_y == pos_y) &&
      map->gb1.available) {
    printf("Found gold!\n");
    return 1;
  }
  if ((map->gb2.pos_x == pos_x) && (map->gb2.pos_y == pos_y) &&
      map->gb2.available) {
    printf("Found gold!\n");
    return 1;
  }

  printf("Gold not found.\n");
  return 0;
} // end hasGold()

// getGold()
// Description: Pick up the gold
void getGold(struct Workspace *map) {

  if ((map->gb1.pos_x == map->wall_e.pos_x) &&
      (map->gb1.pos_y == map->wall_e.pos_y) && map->gb1.available) {
    map->gb1.available = false;
    map->n_gold--;
    map->wall_e.n_goldcollected++;
  }

  if ((map->gb2.pos_x == map->wall_e.pos_x) &&
      (map->gb2.pos_y == map->wall_e.pos_y) && map->gb2.available) {
    map->gb2.available = false;
    map->n_gold--;
    map->wall_e.n_goldcollected++;
  }

  printf("Gold retrieved!\n");
} // end getGold()

// Run4Gold()
// Description: Start the sequence for the Robot to search for gold
void Run4Gold(struct Workspace *map) {
  // Check if there is still golds in the map
  while (MapHasGold(map)) {
    // Update turn counter
    map->turnCount++;
    // debug message, should be commented out if not needed
    // if (test) {printf("There is still gold in the map!\nChecking next
    // square.\n");}

    // start sequence to examine next square and make a move
    checkNextSquare(map);

    if (hasGold(map)) {
      getGold(map);
    }

    printMap(map);
    // created to break out of the loop when debugging
    // if (test) { break;}
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

  printf("\nTurn: %i\n", map->turnCount);
  for (i = 0; i < 4; i++) {
    printf("-----------------\n| %c | %c | %c | %c |\n", map->pos[0][i],
           map->pos[1][i], map->pos[2][i], map->pos[3][i]);
  }
  printf("-----------------\n");
} // end printMap()

// init()
void init(struct Workspace *map) {
  // Initialize blank world
  createWorld(map);
  printf("Initializing to random position");
  // Place everything to random position
  randPos(map);

  map->n_gold = 2;
  
  map->wall_e.n_goldcollected = 0;
  
  map->gb1.available = true;
  map->gb2.available = true;
  
  // Set turn count to zero
  map->turnCount = 0;

} // end init()

/**************************END FUNCTIONS******************************/

/*******************************API***********************************/
void robotAPI(struct Workspace *map){
  init(map);
  printMap(map);
  
  Run4Gold(map);

}
/**************************END API***********************************/

int main() {
  
  struct Workspace map;

  // API Call
  robotAPI(&map);

  printf("\nExiting program...\n");

  return 0;
}

