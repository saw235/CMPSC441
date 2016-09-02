#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
  int n_gold;
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
  gettimeofday(&time, NULL);
  srandom((unsigned int)time.tv_usec);

  for (x = 0; x < 4; y++) {
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
} // end randPos()

// createWorld()
// Description: Initialize the workspace
void createWorld(struct Workspace *map) {

  printf("Populating map with blanks"); // For debugging, remove if not needed

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      map->pos[i][j] = '-';
    }
  }

  // Initialize everything to random position
  // randPos(map);  //Commented out to test cause its not working

} // end createWorld()

// UpdateWorkspace()
// Description: Refreshes the map to reflect the current workspace
void UpdateWorkspace(struct Workspace *map) {

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      map->pos[i][j] = '-';
    }
  }

  // update gold position on map
  if (map->gb1.available) {
    map->pos[map->gb1.pos_x][map->gb1.pos_y] = 'R';
  }
  if (map->gb2.available) {
    map->pos[map->gb2.pos_x][map->gb2.pos_y] = 'R';
  }
  // update wall-e's position on the map
  map->pos[map->wall_e.pos_x][map->wall_e.pos_y] = 'R';

} // end UpdateWorkspace()

// MapHasGold()
// check if there is gold available in the map
bool MapHasGold(struct Workspace *map) {
  return (map->n_gold != 0);
} // end MapHasGold()

// Check next Square
void checkNextSquare() {} // end checkNextSquare()

// hasBomb()
// Description: Check if the square has bomb in it
bool hasBomb() {} // end hasBond

// moveNext()
// Description: Move robot to next square
void moveNext() {} // end moveNext()

// hasGold()
// Description: Check if Robot Stumble Upon Gold
bool hasGold(struct Workspace *map) {
  printf("Checking if there is still gold present in the map.") // For debugging
      int pos_x = map->wall_e.pos_x;
  int pos_y = map->wall_e.pos_y;

  if (map->pos[pos_x][pos_y] == 'G') {
    return 1;
  } else {
    return 0;
  }

} // end hasGold()

// getGold()
// Description: Pick up the gold
void getGold() {
  printf("Picking up gold"); // For debugging
} // end getGold()

// Run4Gold()
// Description: Start the sequence for the Robot to search for gold
void Run4Gold(struct Workspace *map) {
  // Check if there is still golds in the map
  while (!MapHasGold(map)) {
    // debug message, should be commented out if not needed
    printf("Checking next square.\n");

    // start sequence to examine next square
    checkNextSquare();
  }

  // end if no gold
  printf("There is no more gold in the map.\n");
  return;

} // end Run4Gold()

// printMap()
// Description: Output the map in a readable format to the console
void printMap(struct Workspace *map) {
  // char *string[256];
  int i;

  for (i = 0; i < 4; i++) {
    printf("------------------------\n| %c | %c | %c | %c |\n", map->pos[i][0],
           map->pos[i][1], map->pos[i][2], map->pos[i][3]);
  }
  printf("------------------------\n");
} // end printMap()

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

