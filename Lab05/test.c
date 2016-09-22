//Author: Saw Xue Zheng and Corey Zalewski
//Date  : 8 September 2016
//Description : Robot Simulator

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

const bool test = false; // set this to true when debugging to avoid infinite
                        // loop from unfinished code
#define NUM_THREADS 2

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

struct thread_data{
  pthread_t thread_id;
  int thread_num;
  int move_count;
  struct Workspace* map;
}thread_data_array[NUM_THREADS];


pthread_mutex_t map_mutex;

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
void *bombAPI(void*threadarg);
void *robotAPI(void *threadarg);
void run(void* threaddata);
void init_thread_data(struct thread_data* thread, struct Workspace* map);
void StartAPI();
// end Forward Declaration

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




void init_thread_data(struct thread_data* thread, struct Workspace* map)
{
	thread[0].thread_num = 0;
	thread[1].thread_num = 1;

	thread[0].move_count = 0;
	thread[1].move_count = 0;

	thread[0].map = map;
	thread[1].map = map;
}


void run(void* threaddata)
{
	pthread_mutex_lock(&map_mutex);
	pthread_mutex_unlock(&map_mutex);
}

void *bombAPI(void*threadarg)
{

}

void *robotAPI(void *threadarg){

}


void startAPI(){
  struct Workspace map;
  init(&map);
  printMap(&map);

  init_thread_data(thread_data_array, &map);
  
  pthread_mutex_init(&map_mutex, NULL);
  pthread_create(&(thread_data_array[0].thread_id), NULL, robotAPI, (void *)(&thread_data_array[0]));
  pthread_create(&(thread_data_array[1].thread_id), NULL, bombAPI, (void *)(&thread_data_array[1]));


}

int main() {

  // API Call
  startAPI();

  printf("\nExiting program...\n");

  return 0;
}
