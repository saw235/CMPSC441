#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define MAP_WIDTH 5
#define MAP_HEIGHT 5
#define MAP1_NUM_THREADS 4
#define N_objects 7


#define MAP2_WIDTH 2
#define MAP2_HEIGHT 10
#define MAP2_NUM_THREADS 3


struct Tweety{	
   int pos_x;
   int pos_y;
  
   bool won;
   bool dead;
   bool hasFlag;

};

struct Taz{
   int pos_x;
   int pos_y;

   bool dead;
   bool won;
   bool hasFlag;
};

struct Bunny{	
   int pos_x;
   int pos_y;
  
   bool dead;
   bool won;
   bool hasFlag;
};

struct Marvin{
   int pos_x;
   int pos_y;

   bool won;
   bool hasFlag;
   bool bugs;
   bool tty;
   bool tz;
};

struct Flag{
   int pos_x;
   int pos_y;

   bool available;
};

struct Mountain{
   int pos_x;
   int pos_y;
};


struct Workspace {
  char pos[MAP_WIDTH][MAP_HEIGHT];
  int  num_won;
  int  flag_count;
  int  num_ded;
  bool race_won;
  int  turnCount;

  struct Tweety tty;
  struct Taz tz;
  struct Marvin mvin;
  struct Bunny bun;
  struct Flag fg1;
  struct Flag fg2;
  struct Mountain mtn;
   
};

struct thread_data_1{
  pthread_t thread_id;
  int thread_num;
  int move_count;
  struct Workspace* map;
}thread_data1_array[MAP1_NUM_THREADS];




struct timespec sleeptime  = {0, 5000};
pthread_mutex_t map1_mutex;
pthread_mutex_t map2_mutex;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;



int getRandom(int rangeLow, int rangeHigh);
void randPos(struct Workspace *map);
void createWorld(struct Workspace *map);
void updateWorkspace(struct Workspace *map);
void printMap(struct Workspace *map);
bool hasPerson(struct Workspace *map, int x, int y);
bool hasMountain(struct Workspace *map, int x, int y);
bool atMountain(struct Workspace *map, int x, int y);
void randomMove(int *x, int *y);
bool isValidMove(struct Workspace *map, int x, int y, void* threadarg);
void checkNextSquare(struct Workspace *map, void* threadarg);
void moveNext(struct Workspace *map, const int x, const int y, void* threadarg);
bool foundFlag(struct Workspace *map, void* threadarg);
void getFlag(struct Workspace *map, void* threadarg);
void *BugsRun(void* threadarg);
void *TazRun(void* threadarg);
void *TweetyRun(void* threadarg);
void *MarvinRun(void* threadarg);
void init(struct Workspace *map);

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
  
  srand(time(NULL));
  int x[N_objects]; 
  int y[N_objects];
  bool similar = false;
  //check if there are similar positions if yes reroll
  for ( int i = 0; i < N_objects; i++)
    {
       
	do{
	similar = false;
	x[i] = getRandom(0, MAP_WIDTH-1);
	y[i] = getRandom(0, MAP_HEIGHT-1);
	
	//if it is the second
	if (i > 0)
 	{
	    for (int j = 0; j < i ; j++)
	    {
		if ( (x[i] == x[j]) && (y[i] == y[j])) 
		{
		     similar = true;
		}
	    }
	}
	} while (similar);


    }

   //for ( int i = 0; i < N_objects; i++) { printf("%d, %d \n", x[i],y[i]);}

   map->tty.pos_x = x[0];
   map->tty.pos_y = y[0];
   map->pos[x[0]][y[0]] = 'T';

   map->tz.pos_x = x[1];
   map->tz.pos_y = y[1];
   map->pos[x[1]][y[1]] = 'D';

   map->mvin.pos_x = x[2];
   map->mvin.pos_y = y[2];
   map->pos[x[2]][y[2]] = 'M';

   map->bun.pos_x = x[3];
   map->bun.pos_y = y[3];
   map->pos[x[3]][y[3]] = 'B';

   map->fg1.pos_x = x[4];
   map->fg1.pos_y = y[4];
   map->pos[x[4]][y[4]] = 'C';

   map->fg2.pos_x = x[5];
   map->fg2.pos_y = y[5];
   map->pos[x[5]][y[5]] = 'C';

   map->mtn.pos_x = x[6];
   map->mtn.pos_y = y[6];
   map->pos[x[6]][y[6]] = 'F';


} // end randPos()

// createWorld()
// Description: Initialize the workspace
void createWorld(struct Workspace *map) {

  for (int i = 0; i < MAP_WIDTH; i++) {
    for (int j = 0; j < MAP_HEIGHT; j++) {
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
  if (map->fg1.available) {
    map->pos[map->fg1.pos_x][map->fg1.pos_y] = 'C';
  }
  if (map->fg2.available) {
    map->pos[map->fg2.pos_x][map->fg2.pos_y] = 'C';
  }


  map->pos[map->mtn.pos_x][map->mtn.pos_y] = 'F';

  if (!(map->tty.won) && !(map->tty.dead)) {map->pos[map->tty.pos_x][map->tty.pos_y] = 'T';}
  if (!(map->bun.won) && !(map->bun.dead)) {map->pos[map->bun.pos_x][map->bun.pos_y] = 'B';}
  if (!(map->tz.won) && !(map->tz.dead))   {map->pos[map->tz.pos_x][map->tz.pos_y] = 'D';}
  if (!(map->mvin.won)) {map->pos[map->mvin.pos_x][map->mvin.pos_y] = 'M';}

} // end updateWorkspace()

// printMap()
// Description: Output the map in a readable format to the console
void printMap(struct Workspace *map) {

  updateWorkspace(map);
  int i;

  printf("\nTurn: %i\n", map->turnCount);
  
  if (map->bun.hasFlag) { printf("Bugs\t: FLAG\n"); } else {printf("Bugs\t: NONE\n");}
  if (map->tz.hasFlag) { printf("Taz\t: FLAG\n"); } else {printf("Taz\t: NONE\n");}
  if (map->tty.hasFlag) { printf("Tweety\t: FLAG\n"); } else {printf("Tweety\t: NONE\n");}
  if (map->mvin.hasFlag) { printf("Marvin\t: FLAG\n"); } else {printf("Marvin\t: NONE\n");}

  for (i = 0; i < MAP_HEIGHT; i++) {
    printf("---------------------\n| %c | %c | %c | %c | %c |\n", map->pos[0][i],
           map->pos[1][i], map->pos[2][i], map->pos[3][i], map->pos[4][i]);
  }
  printf("---------------------\n");
} // end printMap()



// Description: Check if the square has a person in it
bool hasPerson(struct Workspace *map, int x, int y) {
  bool person = false;
  if (map->pos[x][y] == 'B' || map->pos[x][y] == 'D' || map->pos[x][y] == 'M' || map->pos[x][y] == 'T') {
    person = true;
    printf("There is someone in the square, not moving.\n");
  }
  return person;
} // end hasPerson()


// Description: Check if the square has a Mountain in it
bool hasMountain(struct Workspace *map, int x, int y) {
  bool mtn = false;
  if (map->pos[x][y] == 'F') {
    mtn = true;
  }
  return mtn;
} // end hasMountain()


bool invalidMountain(struct Workspace *map, int pos_x, int pos_y)
{ 
  bool invalid = false;
  
  
  if(pos_x == map->bun.pos_x && pos_y == map->bun.pos_y && !(map->bun.dead))
  {
	invalid = true;
  }
  else if(pos_x == map->tz.pos_x && pos_y == map->tz.pos_y && !(map->tz.dead))
  {
	invalid = true;
  }
  else if(pos_x == map->tty.pos_x && pos_y == map->tty.pos_y && !(map->tty.dead))
  {
	invalid = true;
  }
  else if(pos_x == map->mvin.pos_x && pos_y == map->mvin.pos_y)
  {
	invalid = true;
  }
  else if(pos_x == map->fg1.pos_x && pos_y == map->fg1.pos_y && map->fg1.available)
  {
	invalid = true;
  }
  else if(pos_x == map->fg2.pos_x && pos_y == map->fg2.pos_y && map->fg2.available)
  {
	invalid = true;
  }



  return invalid;
		
}

void randomMountain(struct Workspace *map){
	
     int pos_x;
     int pos_y;

     do{
     pos_x = getRandom(0, MAP_WIDTH-1);
     pos_y = getRandom(0, MAP_HEIGHT-1);
     } while (invalidMountain(map, pos_x,pos_y));

     map->mtn.pos_x = pos_x;
     map->mtn.pos_y = pos_y;
     printf("Mountain teleported to (%d,%d)!\n",map->mtn.pos_x, map->mtn.pos_y);
}

// Description: Check if the square has a Mountain in it
bool hasFlag(struct Workspace *map, int x, int y) {
  bool fg = false;
  if (map->pos[x][y] == 'C') {
    fg = true;
  }
  return fg;
} // end hasFlag()


// Description: Check if the square has a Mountain in it
bool atMountain(struct Workspace *map, int x, int y) {
  bool mtn = false;
  if ((map->mtn.pos_x == x) && (map->mtn.pos_y == y)) {
    mtn = true;
  }
  return mtn;
} // end hasMountain()

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


// isValidMove()
// Description: A move is valid if the Robot is not on a someone else and not out of
// range of the map
bool isValidMove(struct Workspace *map, int x, int y, void* threadarg) {
  bool valid;
  struct thread_data_1* t_dat = (struct thread_data_1*) threadarg;

if (t_dat->thread_num == 0){
  if ((map->bun.pos_x + x >= MAP_WIDTH) || (map->bun.pos_x + x) < 0) {
    valid = false;
  } // check if x is out of range
  else if ((map->bun.pos_y + y >= MAP_HEIGHT) || (map->bun.pos_y + y < 0)) {
    valid = false;
  } // check if y is out of range
  else if (hasPerson(map, map->bun.pos_x + x, map->bun.pos_y + y)) {
    valid = false;
  } 
  else if (hasFlag(map, map->bun.pos_x + x, map->bun.pos_y + y) && map->bun.hasFlag) {
    valid = false;
  }   
  else if (hasMountain(map, map->bun.pos_x + x, map->bun.pos_y + y) && !(map->bun.hasFlag)){
    valid = false;
  }
  else {
    valid = true;
    printf("Safe to move.\n");
  }
}
else if (t_dat->thread_num == 1){
  if ((map->tz.pos_x + x >= MAP_WIDTH) || (map->tz.pos_x + x) < 0) {
    valid = false;
  } // check if x is out of range
  else if ((map->tz.pos_y + y >= MAP_HEIGHT) || (map->tz.pos_y + y < 0)) {
    valid = false;
  } // check if y is out of range
  else if (hasPerson(map, map->tz.pos_x + x, map->tz.pos_y + y)) {
    valid = false;
  }
  else if (hasFlag(map, map->tz.pos_x + x, map->tz.pos_y + y) && map->tz.hasFlag) {
    valid = false;
  }  
  else if (hasMountain(map, map->tz.pos_x + x, map->tz.pos_y + y) && !(map->tz.hasFlag)){
    valid = false;
  }  
  else {
    valid = true;
    printf("Safe to move.\n");
  }
}
else if (t_dat->thread_num == 2){
  if ((map->tty.pos_x + x >= MAP_WIDTH) || (map->tty.pos_x + x) < 0) {
    valid = false;
  } // check if x is out of range
  else if ((map->tty.pos_y + y >= MAP_HEIGHT) || (map->tty.pos_y + y < 0)) {
    valid = false;
  } // check if y is out of range
  else if (hasPerson(map, map->tty.pos_x + x, map->tty.pos_y + y)) {
    valid = false;
  }
  else if (hasFlag(map, map->tty.pos_x + x, map->tty.pos_y + y) && map->tty.hasFlag) {
    valid = false;
  }  
  else if (hasMountain(map, map->tty.pos_x + x, map->tty.pos_y + y) && !(map->tty.hasFlag)){
    valid = false;
  } 
  else {
    valid = true;
    printf("Safe to move.\n");
  }
}
else if (t_dat->thread_num == 3){
  if ((map->mvin.pos_x + x >= MAP_WIDTH) || (map->mvin.pos_x + x) < 0) {
    valid = false;
  } // check if x is out of range
  else if ((map->mvin.pos_y + y >= MAP_HEIGHT) || (map->mvin.pos_y + y < 0)) {
    valid = false;
  } // check if y is out of range
  else if (hasMountain(map, map->mvin.pos_x + x, map->mvin.pos_y + y) && !(map->mvin.hasFlag)){
    valid = false;
  }
  else if (hasFlag(map, map->mvin.pos_x + x, map->mvin.pos_y + y) && map->mvin.hasFlag) {
    valid = false;
  }  
  else if (hasPerson(map, map->mvin.pos_x + x, map->mvin.pos_y + y)) {
    valid = true;
    printf("Marvin found someone! Something bad is about to happen...\n");
  } 
  else {
    valid = true;
    printf("Safe to move.\n");
  }
}
  // printf("\n Move is %d \n", valid);
  return valid;

}

// checkNextSquare()
// Description: Check next square for possible movement
void checkNextSquare(struct Workspace *map, void* threadarg) {

  // Possible move range for x and y are -1 to 1
  int x, y;

  // keep checking for possible move until we found one
  while (true) {

    x = y = 0;
    // generate a random direction
    randomMove(&x, &y);

    // if the move is valid then break out of loop
    if (isValidMove(map, x, y, threadarg)) {
      break;
    }
    // if (test) {break;} //should be commented out when ifValidMove() is
    // working
  }

  // if a valid move is found then to next square

  moveNext(map, x, y, threadarg);

} // end checkNextSquare()

// moveNext()
// Description: Move robot to next square
void moveNext(struct Workspace *map, const int x, const int y, void* threadarg) {

  struct thread_data_1* t_dat = (struct thread_data_1*) threadarg;

  if (t_dat->thread_num == 0){
	map->bun.pos_x = map->bun.pos_x + x;
  	map->bun.pos_y = map->bun.pos_y + y;
       
        printf("Bugs Bunny is now at (%d,%d)\n", map->bun.pos_x + 1, map->bun.pos_y + 1);

  }
  else if (t_dat->thread_num == 1){
	map->tz.pos_x = map->tz.pos_x + x;
	map->tz.pos_y = map->tz.pos_y + y;

	printf("Taz Devil is now at (%d,%d)\n", map->tz.pos_x + 1, map->tz.pos_y + 1);

  }
  else if (t_dat->thread_num == 2){
	map->tty.pos_x = map->tty.pos_x + x;
	map->tty.pos_y = map->tty.pos_y + y;

	printf("Tweety is now at (%d,%d)\n", map->tty.pos_x + 1, map->tty.pos_y + 1);

  }
  else if (t_dat->thread_num == 3){
	map->mvin.pos_x = map->mvin.pos_x + x;
	map->mvin.pos_y = map->mvin.pos_y + y;

	printf("Marvin is now at (%d,%d)\n", map->mvin.pos_x + 1, map->mvin.pos_y + 1);

  }

} // end moveNext()


bool foundPerson(struct Workspace *map, void* threadarg)
{
    updateWorkspace(map); 


  int pos_x; 
  int pos_y;
  bool found = false;
  
  map->mvin.tty =false;
  map->mvin.bugs =false;
  map->mvin.tz =false;

  struct thread_data_1* t_dat = (struct thread_data_1*) threadarg;

  if (t_dat->thread_num == 0)
  {	
      pos_x = map->bun.pos_x;
      pos_y = map->bun.pos_y;
  }
  else if (t_dat->thread_num == 1)
  {	
      pos_x = map->tz.pos_x;
      pos_y = map->tz.pos_y;
  } 
  else if (t_dat->thread_num == 2)
  {	
      pos_x = map->tty.pos_x;
      pos_y = map->tty.pos_y;
  }
  else if (t_dat->thread_num == 3)
  {	
      pos_x = map->mvin.pos_x;
      pos_y = map->mvin.pos_y;
  }
    
   
  if (map->tty.pos_x == pos_x && map->tty.pos_y == pos_y && !(map->tty.dead) && !(map->tty.won))
     {
	printf("Found Tweety!...\n");
	map->mvin.tty = true;
        found = true;
     }
  else if (map->bun.pos_x == pos_x && map->bun.pos_y == pos_y && !(map->bun.dead) && !(map->bun.won))
     {
	printf("Found Bugs bunny!...\n");
	map->mvin.bugs = true;
        found = true;
     }
  else if (map->tz.pos_x == pos_x && map->tz.pos_y == pos_y && !(map->tz.dead) && !(map->tz.won))
     {
	printf("Found Taz Devil!...\n");
	map->mvin.tz = true;
        found = true;
     }

   return found;
}


bool foundPersonhasFlag(struct Workspace *map)
{
	if (map->mvin.tty && map->tty.hasFlag) {return true;}
	if (map->mvin.bugs && map->bun.hasFlag) {return true;}
	if (map->mvin.tz && map->tz.hasFlag) {return true;}

	return false;
}

//only for marvin
void stealfoundPersonFlag(struct Workspace *map)
{
	if (map->mvin.hasFlag) { map->flag_count -= 1; printf("Marvin already has a flag. Steal anyway!\n"); }

	if (map->mvin.tty) { map->tty.hasFlag = false; map->mvin.hasFlag = true; printf("Tweety's flag is stolen!");}
	if (map->mvin.bugs) { map->bun.hasFlag = false; map->mvin.hasFlag = true; printf("Bug's flag is stolen!");}
	if (map->mvin.tz) { map->tz.hasFlag = false; map->mvin.hasFlag = true; printf("Taz's flag is stolen!");}

	
}

//only for marvin
void killfoundPerson(struct Workspace *map)
{
	if (map->mvin.tty) { map->tty.dead = true; map->num_ded += 1; printf("Tweety is eliminated!");}
	if (map->mvin.bugs) { map->bun.dead = true; map->num_ded += 1; printf("Bugs bunny is eliminated!");}
	if (map->mvin.tz) { map->tz.dead = true; map->num_ded += 1; printf("Taz Devil is eliminated!");}
}

bool foundFlag(struct Workspace *map, void* threadarg) {
  //printf("Checking if Flag is encountered.\n"); // For debugging

  updateWorkspace(map);
  int pos_x; 
  int pos_y;

  struct thread_data_1* t_dat = (struct thread_data_1*) threadarg;

  if (t_dat->thread_num == 0)
  {	
      pos_x = map->bun.pos_x;
      pos_y = map->bun.pos_y;
  }
  else if (t_dat->thread_num == 1)
  {	
      pos_x = map->tz.pos_x;
      pos_y = map->tz.pos_y;
  } 
  else if (t_dat->thread_num == 2)
  {	
      pos_x = map->tty.pos_x;
      pos_y = map->tty.pos_y;
  }
  else if (t_dat->thread_num == 3)
  {	
      pos_x = map->mvin.pos_x;
      pos_y = map->mvin.pos_y;
  }
 

  if ((map->fg1.pos_x == pos_x) && (map->fg1.pos_y == pos_y) &&
      map->fg1.available) {
    printf("Found flag!\n");
    return 1;
  }
  if ((map->fg2.pos_x == pos_x) && (map->fg2.pos_y == pos_y) &&
      map->fg2.available) {
    printf("Found flag!\n");
    return 1;
  }

  //printf("Flag not found.\n");
  return 0;
} // end foundFlag()

// getFlag()
// Description: Pick up the flag
void getFlag(struct Workspace *map, void* threadarg) {

  int pos_x; 
  int pos_y;

  struct thread_data_1* t_dat = (struct thread_data_1*) threadarg;

  if (t_dat->thread_num == 0)
  {	
      pos_x = map->bun.pos_x;
      pos_y = map->bun.pos_y;

      map->bun.hasFlag = true;
  }
  else if (t_dat->thread_num == 1)
  {	
      pos_x = map->tz.pos_x;
      pos_y = map->tz.pos_y;

      map->tz.hasFlag = true;
  } 
  else if (t_dat->thread_num == 2)
  {	
      pos_x = map->tty.pos_x;
      pos_y = map->tty.pos_y;

      map->tty.hasFlag = true;
  }
  else if (t_dat->thread_num == 3)
  {	
      pos_x = map->mvin.pos_x;
      pos_y = map->mvin.pos_y;
      
      map->mvin.hasFlag = true;
  }


  if ((map->fg1.pos_x == pos_x) &&
      (map->fg1.pos_y == pos_y) && map->fg1.available) {
    map->fg1.available = false;
  }

  if ((map->fg2.pos_x == pos_x) &&
      (map->fg2.pos_y == pos_y) && map->fg2.available) {
    map->fg2.available = false;

  }

  printf("Flag retrieved!\n");
} // end getGold()




void *BugsRun(void* threadarg) {
  
   struct thread_data_1* t_dat = (struct thread_data_1*) threadarg;
   
  while (1) {
	
    pthread_mutex_lock(&map1_mutex);

    
    if (t_dat->map->num_won == t_dat->map->flag_count || t_dat->map->bun.dead){ 
	printf("Exiting bugs thread.\n");    
	pthread_mutex_unlock(&map1_mutex);
	pthread_exit(NULL);}
    
    // Update turn counter
    t_dat->map->turnCount++;
    // debug message, should be commented out if not needed
    // if (test) {printf("There is still gold in the map!\nChecking next
    // square.\n");}

    // start sequence to examine next square and make a move
    
    if (t_dat->map->turnCount % 3 == 0) { 
	
    printf("Activate Time Machine!\n");
    randomMountain(t_dat->map);
    
    }
    



    checkNextSquare(t_dat->map, threadarg);

    if (foundFlag(t_dat->map, threadarg)) {
      getFlag(t_dat->map, threadarg);
    }

    if (atMountain(t_dat->map, t_dat->map->bun.pos_x, t_dat->map->bun.pos_y))
	{ 
	   if (t_dat->map->bun.hasFlag) 
		{t_dat->map->num_won += 1; t_dat->map->bun.won = true; break;}
	}

    printMap(t_dat->map);
    
    
    // created to break out of the loop when debugging
    // if (test) { break;}

    pthread_mutex_unlock(&map1_mutex);
    nanosleep(&sleeptime,NULL);

  }

  pthread_mutex_unlock(&map1_mutex);

  printMap(t_dat->map);
  printf("Bugs bunny won the first race!.\n");
  printf("Bugs bunny joins the second race!.\n");
  pthread_exit(NULL);

}

void *TazRun(void* threadarg) {
     struct thread_data_1* t_dat = (struct thread_data_1*) threadarg;
   
  while (1) {
	
    pthread_mutex_lock(&map1_mutex);

    
    if (t_dat->map->num_won == t_dat->map->flag_count  || t_dat->map->tz.dead){ 
	printf("Exiting Taz thread.\n");    
	pthread_mutex_unlock(&map1_mutex);
	pthread_exit(NULL);}
    
	
    // Update turn counter
    t_dat->map->turnCount++;
    // debug message, should be commented out if not needed
    // if (test) {printf("There is still gold in the map!\nChecking next
    // square.\n");}

    if (t_dat->map->turnCount % 3 == 0) { 
	
    printf("Activate Time Machine!\n");
    randomMountain(t_dat->map);
    
    }

    // start sequence to examine next square and make a move
    checkNextSquare(t_dat->map, threadarg);

    if (foundFlag(t_dat->map, threadarg)) {
      getFlag(t_dat->map, threadarg);
    }

    if (atMountain(t_dat->map, t_dat->map->tz.pos_x, t_dat->map->tz.pos_y))
	{ 
	   if (t_dat->map->tz.hasFlag) 
		{t_dat->map->num_won += 1; t_dat->map->tz.won = true; break;}
	}

    printMap(t_dat->map);
    
    
    // created to break out of the loop when debugging
    // if (test) { break;}

    pthread_mutex_unlock(&map1_mutex);
    nanosleep(&sleeptime,NULL);

  }

  pthread_mutex_unlock(&map1_mutex);

  printMap(t_dat->map);
  printf("Taz Devil won the first race!.\n");
  printf("Taz Devil joins the second race!.\n");  
  pthread_exit(NULL);

}

void *TweetyRun(void* threadarg) {
   struct thread_data_1* t_dat = (struct thread_data_1*) threadarg;
   
  while (1) {
	
    pthread_mutex_lock(&map1_mutex);

    
    if (t_dat->map->num_won == t_dat->map->flag_count  || t_dat->map->tty.dead){ 
	printf("Exiting tweety thread.\n");    
	pthread_mutex_unlock(&map1_mutex);
	pthread_exit(NULL);}
    
	
    // Update turn counter
    t_dat->map->turnCount++;
    // debug message, should be commented out if not needed
    // if (test) {printf("There is still gold in the map!\nChecking next
    // square.\n");}

    if (t_dat->map->turnCount % 3 == 0) { 
	
    printf("Activate Time Machine!\n");
    randomMountain(t_dat->map);
    
    }

    // start sequence to examine next square and make a move
    checkNextSquare(t_dat->map, threadarg);

    if (foundFlag(t_dat->map, threadarg)) {
      getFlag(t_dat->map, threadarg);
    }

    if (atMountain(t_dat->map, t_dat->map->tty.pos_x, t_dat->map->tty.pos_y))
	{ 
	   if (t_dat->map->tty.hasFlag) 
		{t_dat->map->num_won += 1; t_dat->map->tty.won = true; break;}
	}

    printMap(t_dat->map);
    
    
    // created to break out of the loop when debugging
    // if (test) { break;}

    pthread_mutex_unlock(&map1_mutex);
    nanosleep(&sleeptime,NULL);

  }

  pthread_mutex_unlock(&map1_mutex);

  printMap(t_dat->map);
  printf("Tweety won the first race!.\n");
  printf("Tweety joins the second race!.\n"); 
  pthread_exit(NULL);

}

void *MarvinRun(void* threadarg) {
   struct thread_data_1* t_dat = (struct thread_data_1*) threadarg;
   
  while (1) {
	
    pthread_mutex_lock(&map1_mutex);

    if (t_dat->map->num_won == t_dat->map->flag_count  ){ 
	printf("Exiting Marvin thread.\n");    
	pthread_mutex_unlock(&map1_mutex);
	pthread_exit(NULL);}
    
	
    // Update turn counter
    t_dat->map->turnCount++;
    // debug message, should be commented out if not needed
    // if (test) {printf("There is still gold in the map!\nChecking next
    // square.\n");}
    if (t_dat->map->turnCount % 3 == 0) { 
	
    printf("Activate Time Machine!\n");
    randomMountain(t_dat->map);
    
    }

    // start sequence to examine next square and make a move
    checkNextSquare(t_dat->map, threadarg);
    
    if (foundPerson(t_dat->map, threadarg))
    {
	if (foundPersonhasFlag(t_dat->map))
		{ stealfoundPersonFlag(t_dat->map);}
	killfoundPerson(t_dat->map);
    }

    if (foundFlag(t_dat->map, threadarg)) {
      getFlag(t_dat->map, threadarg);
    }

    if (atMountain(t_dat->map, t_dat->map->mvin.pos_x, t_dat->map->mvin.pos_y))
	{ 
	   if (t_dat->map->mvin.hasFlag) 
		{t_dat->map->num_won += 1; t_dat->map->mvin.won = true; break;}
	}

    printMap(t_dat->map);
    
    
    // created to break out of the loop when debugging
    // if (test) { break;}

    pthread_mutex_unlock(&map1_mutex);
    nanosleep(&sleeptime,NULL);

  }

  pthread_mutex_unlock(&map1_mutex);

  printMap(t_dat->map);
  printf("Marvin won the first race!.\n");
  printf("Marvin joins the second race!.\n"); 
  printf("Exiting Marvin thread.\n"); 
  pthread_exit(NULL);
}

// init()
void init(struct Workspace *map) {
  // Initialize blank world
  createWorld(map);
  printf("Initializing to random position");
  // Place everything to random position
  randPos(map);
 
  map->race_won = false;
  map->flag_count = 2;
  map->num_won = 0;
  map->tty.won = false;
  map->tz.won = false;
  map->mvin.won = false;
  map->bun.won = false;
  
  map->num_ded = 0;
  map->tty.dead = false;
  map->tz.dead = false;
  map->bun.dead = false;
  
  map->tty.hasFlag = false;
  map->tz.hasFlag = false;
  map->mvin.hasFlag = false;
  map->bun.hasFlag = false;  

  map->fg1.available = true;
  map->fg2.available = true;

  map->mvin.bugs = false;
  map->mvin.tty = false;
  map->mvin.tz = false;
  // Set turn count to zero
  map->turnCount = 0;

} // end init()

void init_thread_data_1(struct thread_data_1* thread, struct Workspace* map)
{
	for (int i = 0; i < MAP1_NUM_THREADS; i++)
	{
		thread[i].thread_num = i;
		thread[i].move_count = 0;
		thread[i].map = map;	
	}

}


/*************************************************************************************************************************
***********************************************CODES for RACE2 *********************************************************
***********************************************************************************************************************/

struct ToonX{
   int char_id;
   char name[20];	
   int pos_x;
   int pos_y;
  
   bool frozen;
   bool won;

};

struct ToonY{
   int char_id;
   char name[20];
   int pos_x;
   int pos_y;


   bool frozen;
   bool won;
};

struct Workspace2 {
  char pos[MAP2_WIDTH][MAP2_HEIGHT];
  int turn_count;
  bool justMar;

  struct ToonX tX;
  struct ToonY tY;

};

struct thread_data_2{
  pthread_t thread_id;
  int thread_num;
  int move_count;
  struct Workspace2* map;
}thread_data2_array[MAP2_NUM_THREADS];


void createWorld2(struct Workspace2*);
void printMap2(struct Workspace2*);
void updateWorkspace2(struct Workspace2 *); 
void init2(struct Workspace2 *);

/**************************FUNCTIONS******************************/


// createWorld2()
// Description: Initialize the workspace
void createWorld2(struct Workspace2 *map) {

  for (int i = 0; i < MAP2_WIDTH; i++) {
    for (int j = 0; j < MAP2_HEIGHT; j++) {
      map->pos[i][j] = '-';
    }
  }

} // end createWorld()

// printMap2()
// Description: Output the map in a readable format to the console
void printMap2(struct Workspace2 *map) {

  updateWorkspace2(map);
  int j;
  char tXstring[20];
  char tYstring[20];
  printf("\nTurn: %i\n", map->turn_count);

	
  if (map->tX.frozen) { strcpy(tXstring, "Frozen");}
  else { strcpy(tXstring, "Normal");}
  
  if (!map->justMar)
  {
  if (map->tY.frozen) { strcpy(tYstring, "Frozen");}
  else { strcpy(tYstring, "Normal");}
  

  printf("%s: %s  %s: %s\n", map->tX.name,tXstring,map->tY.name, tYstring); 
  for (j = 0; j < MAP_HEIGHT; j++) {
    printf("-----------------\n| %c | %c |\n", map->pos[0][j],
           map->pos[1][j]);
  }
  printf("-----------------\n");


  } else {
  printf("%s: %s \n", map->tX.name,tXstring); 
  for (j = 0; j < MAP_HEIGHT; j++) {
    printf("-----------------\n| %c | %c |\n", map->pos[0][j],
           map->pos[1][j]);
  }
  printf("-----------------\n");
  }




} // end printMap()


// Description: Refreshes the map to reflect the current workspace
void updateWorkspace2(struct Workspace2 *map) {

  // use createWorld() to create fill with blanks
  createWorld2(map);

  // update position on the map

  if (map->tX.char_id == 2) { map->pos[map->tX.pos_x][map->tX.pos_y] = 'D';}
  else {map->pos[map->tX.pos_x][map->tX.pos_y] = map->tX.name[0];}

  if (!map->justMar)
  {
  if (map->tY.char_id == 2) { map->pos[map->tY.pos_x][map->tY.pos_y] = 'D';}
  else {map->pos[map->tY.pos_x][map->tY.pos_y] = map->tY.name[0];}
  }

} // end updateWorkspace()




// init()
void init2(struct Workspace2 *map) {
  // Initialize blank world
  createWorld2(map);
  
  map->tX.pos_x = 0;
  map->tX.pos_y = 0;
  map->tX.frozen = false;
  map->tX.won = false;

  map->tY.pos_x = 1;
  map->tY.pos_y = 0;
  map->tY.frozen = false;
  map->tY.won = false;
  


  switch (map->tX.char_id)
  {
	case 0 : strcpy(map->tX.name, "Tweety"); break;
	case 1 : strcpy(map->tX.name, "Bugs"); break;
	case 2 : strcpy(map->tX.name, "Taz"); break;
	case 3 : strcpy(map->tX.name, "Marvin"); break;
  }

  if (!map->justMar)
  {
    switch (map->tY.char_id)
  {
	case 0 : strcpy(map->tY.name, "Tweety"); break;
	case 1 : strcpy(map->tY.name, "Bugs"); break;
	case 2 : strcpy(map->tY.name, "Taz"); break;
	case 3 : strcpy(map->tY.name, "Marvin"); break;
  }
  }

  // Set turn count to one
  map->turn_count = 1;

} // end init()

void init_thread_data_2(struct thread_data_2* thread, struct Workspace2* map)
{
	for (int i = 0; i < MAP2_NUM_THREADS; i++)
	{
		thread[i].thread_num = i;
		thread[i].map = map;	
	}
	
}

void* tXRun(void* threadarg)
{

	struct thread_data_2* t_dat = (struct thread_data_2*) threadarg;


	int maxturn;

	if (t_dat->map->justMar) { maxturn = 2;}
	else { maxturn = 3;}

	while(1)
	{
		pthread_mutex_lock(&map2_mutex);
		//printf("in tweety Thread\n");
		if (t_dat->map->tY.won)
			{
				printf("Exiting %s thread.\n", t_dat->map->tX.name);
       				pthread_mutex_unlock(&map2_mutex);
        			pthread_exit(NULL);
			}

		if (t_dat->map->turn_count%maxturn == 1){
			
			//check if frozen
			if (!t_dat->map->tX.frozen)
			{
				//if not frozen, move
				t_dat->map->tX.pos_y += 1;
			} else { t_dat->map->tX.frozen = false;}
			
			if (t_dat->map->tX.pos_y == MAP_HEIGHT)
			{
			    t_dat->map->tX.won = true;
			    break;
			}
			
			
			t_dat->map->turn_count += 1; 

			printMap2(t_dat->map);
		}

		pthread_mutex_unlock(&map2_mutex);
		nanosleep(&sleeptime, NULL);	
	}

	pthread_mutex_unlock(&map2_mutex);
	nanosleep(&sleeptime, NULL);

	printf("%s won!\n",t_dat->map->tX.name);

	pthread_exit(NULL);

	
}

void* tYRun(void* threadarg)
{
	struct thread_data_2* t_dat = (struct thread_data_2*) threadarg;

	while(1)
	{	
		pthread_mutex_lock(&map2_mutex);
		//printf("in Muttley Thread\n");
		if (t_dat->map->tX.won == true)
			{
				printf("Exiting %s thread.\n" ,t_dat->map->tY.name);
       				pthread_mutex_unlock(&map2_mutex);
        			pthread_exit(NULL);
			}
		
		   if (t_dat->map->turn_count%3 == 2){


			//check if frozen
			if (!t_dat->map->tY.frozen)
			{
				//if not frozen, move
				t_dat->map->tY.pos_y += 1;
			}
			else { t_dat->map->tY.frozen = false;}
			
			if (t_dat->map->tY.pos_y == MAP_HEIGHT)
			{
			    t_dat->map->tY.won = true;
			    break;
			}

			t_dat->map->turn_count += 1; 

		printMap2(t_dat->map);	
		}
			
		pthread_mutex_unlock(&map2_mutex);
		nanosleep(&sleeptime, NULL);	
	}

	
	pthread_mutex_unlock(&map2_mutex);
	nanosleep(&sleeptime, NULL);

	printf("%s won!\n" ,t_dat->map->tY.name);

	pthread_exit(NULL);

}

void* SAMZap(void* threadarg)
{

	struct thread_data_2* t_dat = (struct thread_data_2*) threadarg;

	int maxturn;

	if (t_dat->map->justMar) { maxturn = 2;}
	else { maxturn = 3;}

	while(1)
	{	
		pthread_mutex_lock(&map2_mutex);
		//printf("in Marvin Thread\n");
		bool zap;
		int m_or_t;
		if (t_dat->map->tX.won || t_dat->map->tY.won)
		{
			printf("Exiting SAM thread.\n");
       			pthread_mutex_unlock(&map2_mutex);
        		pthread_exit(NULL);
		}
		if (t_dat->map->turn_count%maxturn == 0){
			printf("SAM: ZAP!!!\n");
			

			//if odd
			if (t_dat->map->turn_count %2)
				{	
					zap = false;
					printf("AWW, MISSED!!!\n");
				}
			else {
				zap = getRandom(0,1);

				if (!t_dat->map->justMar)
				{
					m_or_t = getRandom(0,1);
				} else { m_or_t = 0;}

				switch (m_or_t)
				{

					case 0: 
						t_dat->map->tX.frozen = (bool)zap;
						if (zap) { printf("%s zapped!\n",t_dat->map->tX.name );}
						else {printf("Missed %s.\n",t_dat->map->tX.name);}

						break;
					case 1: 
						t_dat->map->tY.frozen = (bool)zap; 
						if (zap) { printf("%s zapped!\n",t_dat->map->tY.name);}
						else {printf("Missed %s.\n",t_dat->map->tY.name);}
						break;
					default: break;
				}
			}

 			t_dat->map->turn_count += 1; 
			printMap2(t_dat->map);	
		}
		
		pthread_mutex_unlock(&map2_mutex);
		nanosleep(&sleeptime, NULL);	
	}
}



void StartAPI(){
  struct Workspace map;
  init(&map);
  printMap(&map);

  init_thread_data_1(thread_data1_array, &map);
  
  pthread_mutex_init(&map1_mutex, NULL);
  pthread_create(&(thread_data1_array[0].thread_id), NULL, BugsRun, (void *)(&thread_data1_array[0]));
  pthread_create(&(thread_data1_array[1].thread_id), NULL, TazRun, (void *)(&thread_data1_array[1]));
  pthread_create(&(thread_data1_array[2].thread_id), NULL, TweetyRun, (void *)(&thread_data1_array[2]));
  pthread_create(&(thread_data1_array[3].thread_id), NULL, MarvinRun, (void *)(&thread_data1_array[3]));

  for (int i = 0; i < MAP1_NUM_THREADS; i++)
  {
  	pthread_join(thread_data1_array[i].thread_id,NULL);
  }

  pthread_mutex_destroy(&map1_mutex); 

  
  if (map.num_won == 2)
  {

	struct Workspace2 map2;

	int won[4] = {0,0,0,0};
	if (map.tty.won) { won[0] = 1;}
	if (map.bun.won) { won[1] = 1;}
	if (map.tz.won) { won[2] = 1;}
	if (map.mvin.won) { won[3] = 1;}

	//assign id for toon x
	for (int i = 0; i < 4; i++)
	{
	    if (won[i])
		{
			map2.tX.char_id = i;
			break;
		}   
	}

	//assign id for toon y
	for (int i = 0; i < 4; i++)
	{
	    if (won[i])
		{
			//check if same as toonx
			if (map2.tX.char_id == i)
			{
				continue;
			}
			else 
			{	
				map2.tY.char_id = i;
				break;
			}
		}   
	}
	
	map2.justMar = false;
	init2(&map2);
	printMap2(&map2);	

	init_thread_data_2(thread_data2_array, &map2);
	pthread_mutex_init(&map2_mutex, NULL);
	pthread_create(&(thread_data2_array[0].thread_id), NULL, tXRun, (void *)(&thread_data2_array[0]));	
	pthread_create(&(thread_data2_array[1].thread_id), NULL, tYRun, (void *)(&thread_data2_array[1]));	
	pthread_create(&(thread_data2_array[2].thread_id), NULL, SAMZap, (void *)(&thread_data2_array[2]));	


	for (int i = 0; i < MAP2_NUM_THREADS; i++)
  	{
  	pthread_join(thread_data2_array[i].thread_id,NULL);
  	}
  	pthread_mutex_destroy(&map2_mutex); 
  }
  else if (map.num_won == 1)
	{
	struct Workspace2 map2;

	int won[4] = {0,0,0,0};
	if (map.tty.won) { won[0] = 1;}
	if (map.bun.won) { won[1] = 1;}
	if (map.tz.won)  { won[2] = 1;}
	if (map.mvin.won) { won[3] = 1;}

	for (int i = 0; i < 4 ; i++)
 	{
	   printf("%d", won[i]);
	}
	 printf("\n");
	//assign id for toon x
	for (int i = 0; i < 4; i++)
	{
	    if (won[i])
		{
			map2.tX.char_id = i;
			break;
		}   
	}
	
	map2.justMar = true;

	init2(&map2);
	printMap2(&map2);	

	
	init_thread_data_2(thread_data2_array, &map2);
	pthread_mutex_init(&map2_mutex, NULL);
	pthread_create(&(thread_data2_array[0].thread_id), NULL, tXRun, (void *)(&thread_data2_array[0]));	
	pthread_create(&(thread_data2_array[2].thread_id), NULL, SAMZap, (void *)(&thread_data2_array[2]));	
	

	
  	pthread_join(thread_data2_array[0].thread_id,NULL);
  	pthread_join(thread_data2_array[2].thread_id,NULL);

  	pthread_mutex_destroy(&map2_mutex); 
	
	}

}

int main()
{
     // API Call
  StartAPI();

  printf("\nExiting program...\n");

  return 0;
}

