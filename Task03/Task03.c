//Author: Saw Xue Zheng and Corey Zalewski
//Date  : 9 November 2016
//Description : The ultimate race for Toons

// TODO: Have at least two winners, toons can only pick up one carrot, and have a check to see if Marvin killed everyone. Implement the second race.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

// Structs
typedef struct thread{
  pthread_t thread_id;
  int id;
  int condition, x, y, copy_goal, copy_cycle;
  int carrot;
  char name[7], letter;
  
} thread_data;

struct Shared{
  int condition_t;
  int goal_t;
  int eliminated_t[3];
  int cycle_t;
  int carrot_t[2][2];
  int carrot_holder_t[2];
  int mtn_t[2];
  char winner_t[2][7];
  char map[5][5];
}shared_t;

pthread_mutex_t timeTravel_signal_mutex;

// Function Headers
void setup_time_seed();
int getRandom(int rangeLow, int rangeHigh);
void *run_API(void *thread);
void runner_signal(thread_data *runner);
void init_data(thread_data *thread);
void print_map();
bool valid_move(char c, int x, int y);
void check_pos(thread_data *runner, int x, int y);
int check_person(int x, int y);
void update_pos(char c, int xn, int yn, int *xo, int *yo);
void rand_pos(int *x, int *y);
void move_mtn();
void init_pos(thread_data *thread);
void create_map();

// Function Delcarations
void setup_time_seed(){
  struct timeval time;
  gettimeofday(&time, NULL);
  //srandom(time(NULL));
  srandom((unsigned int) time.tv_usec);
}

// Description: Takes in two ranges and return a random number between that.
int getRandom(int rangeLow, int rangeHigh) {
  double myRand = rand() / (1.0 + RAND_MAX);
  int range = rangeHigh - rangeLow + 1;
  int myRand_scaled = (myRand * range) + rangeLow;
  return myRand_scaled;
} // end getRandom()

void print_map(){
  int i, j, k, person, carrots = 0;
  for(i = 0; i < 5; i++){
    printf("\n--------------------------\n|");
    for(j = 0; j < 5; j++){
      carrots = 0;
      printf("%c", shared_t.map[i][j]);
      person = check_person(i, j);
      if(person != -1){
        for(k = 0; k < 2; k++){
          if(shared_t.carrot_holder_t[k] == person)
            carrots++;
        }
      }
      if(carrots == 2){printf("%s", "(K)");}
      else if(carrots == 1){printf("%s", "(C)");}
      else{printf("   ");}
      printf("|");
    }
  }
  printf("\n--------------------------\n");
}

bool valid_move(char c, int x, int y){
  bool valid = true;
  int person, i;
  char letter[] = {'B', 'D', 'T', 'M'};
  for(i = 0; i < 4; i++){
    if(c == letter[i]){
      person = i;
    }
  }
  // not jumping off the map
  if(x < 0 || x > 4 || y < 0 || y > 4){valid = false;}
  // check if mtn is in the way / have the one ring
  else if(shared_t.map[x][y] == 'F' && !(shared_t.carrot_holder_t[0] == person || shared_t.carrot_holder_t[1] == person))
  {valid = false;}
  // check if bumping into other competition / if marvin
  else if(shared_t.map[x][y] != 'C' && shared_t.map[x][y] != ' ' && person != 3)
  {valid = false;}
  else if((shared_t.carrot_holder_t[0] == person || shared_t.carrot_holder_t[1] == person) && ((shared_t.carrot_t[0][0] == x && shared_t.carrot_t[0][1] == y) || (shared_t.carrot_t[1][0] == x &&    shared_t.carrot_t[1][1] == y)))
  {valid = false;}

  return valid;
}

void check_pos(thread_data *runner, int x, int y){
  int i;
  // check if marvin
  if(runner->id == 3){
    // check if moving on a person
    int person = check_person(x, y);
    if(person != -1){
      shared_t.eliminated_t[person]++;
      if(shared_t.carrot_holder_t[0] == person){runner->carrot++;shared_t.carrot_holder_t[0] = runner->id;}
      else if(shared_t.carrot_holder_t[1] == person){runner->carrot++;shared_t.carrot_holder_t[1] = runner->id;}
    }
  }
  // check for carrot
  for(i = 0; i < 2; i++){
    if(shared_t.carrot_t[i][0] == x && shared_t.carrot_t[i][1] == y){
        // pickup carrot
	shared_t.carrot_holder_t[i] = runner->id;
        runner->carrot++;
        shared_t.carrot_t[i][0] = -1;
        shared_t.carrot_t[i][1] = -1;
    }
  }
}

int check_person(int x, int y){
  int id;
  char person = shared_t.map[x][y];
  if(person == 'B'){id = 0;}
  else if(person == 'D'){id = 1;}
  else if(person == 'T'){id = 2;}
  else if(person == 'M'){id = 3;}
  else{id = -1;}
  return id;
}

void update_pos(char c, int xn, int yn, int *xo, int *yo){
  // replace old spot with ' '
  shared_t.map[*xo][*yo] = ' ';
  // replace new spot with c
  shared_t.map[xn][yn] = c;
  // old pos = new pos
  *xo = xn;
  *yo = yn;
}

void rand_pos(int *x, int *y){
  // get random
  do{
    *x = getRandom(0, 2) - 1;
    *y = getRandom(0, 2) - 1;
  }while((*y == 0) && (*x == 0));
}

void init_pos(thread_data *thread){
  int pos[7][2], i, j;
  char c[7] = {'B', 'D', 'T', 'M', 'C', 'C', 'F'};
  bool taken;
  for(i = 0; i < 7; i++){
    do{
      taken = false;
      pos[i][0] = getRandom(0, 4);
      pos[i][1] = getRandom(0, 4);
      for(j = 0; j < i; j++){
        if(pos[i][0] == pos[j][0] && pos[i][1] == pos[j][1])
          taken = true;
      }
    }while(taken);
    if(i < 4){
      thread[i].x = pos[i][0];
      thread[i].y = pos[i][1];
    }
    else if(i < 6){
      shared_t.carrot_t[i - 4][0] = pos[i][0];
      shared_t.carrot_t[i - 4][1] = pos[i][1];
    }
    else{
      shared_t.mtn_t[0] = pos[i][0];
      shared_t.mtn_t[1] = pos[i][1];
    }
    shared_t.map[pos[i][0]][pos[i][1]] = c[i];
  }
}

void move_mtn(){
  int x, y, moved = 0;
  printf("Moving MTN\n");
  do{
    x = getRandom(0, 4);
    y = getRandom(0, 4);
    if(shared_t.map[x][y] == ' '){
      shared_t.map[shared_t.mtn_t[0]][shared_t.mtn_t[1]] = ' ';
      shared_t.map[x][y] = 'F';
      shared_t.mtn_t[0] = x;
      shared_t.mtn_t[1] = y;
      moved = 1;
    }
  }while(moved == 0);
}

void create_map(){
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 5; j++){
      shared_t.map[i][j] = ' ';
    }
  }
}

void runner_signal(thread_data *runner){
  // Lock it down
  pthread_mutex_lock(&timeTravel_signal_mutex);
  // check if it's allowed to run
  if(shared_t.condition_t == runner->condition){
    int x, y;
    // Print cycle
    printf("Cycle: %d\n",shared_t.cycle_t);
    // Check if Marvin
    if(runner->id == 3){
      // Print runner name
      printf("%s is moving.\n", runner->name);
      // Check if cycle % 12
      if(shared_t.cycle_t % 12 == 0){
        // Move Mountain
        move_mtn();
      }
      // Move Marvin / Eliminate the competition (with or without the carrot)
      do{
        // Move
        x = getRandom(0, 2) - 1 + runner->x;
        y = getRandom(0, 2) - 1 + runner->y;
      }while(!valid_move(runner->letter, x, y));
      // check if carrot or competition to take/takedown
      check_pos(runner, x, y);
      update_pos(runner->letter, x, y, &runner->x, &runner->y);
      
      printf("\n");
      print_map();
    }
    // Not Marvin
    else{
      // Check if terminated
      if(shared_t.eliminated_t[runner->id] == 0){
        // Print runner name
        printf("%s is moving.\n", runner->name);
        do{
          // Move
          x = getRandom(0, 2) - 1 + runner->x;
          y = getRandom(0, 2) - 1 + runner->y;
        }while(!valid_move(runner->letter, x, y));
        update_pos(runner->letter, x, y, &runner->x, &runner->y);
        // check for carrot to take
        check_pos(runner, x, y);

        printf("\n");
        print_map();
      }
    }
    // Check if won
    if(runner->x == shared_t.mtn_t[0] && runner->y == shared_t.mtn_t[1] && runner->carrot > 0){
      shared_t.goal_t = 1;
      strcpy(shared_t.winner_t[0], runner->name);
    }

    // Update goal
    runner->copy_goal = shared_t.goal_t;
    // Update cycle
    shared_t.cycle_t++;
    // Update condition
    shared_t.condition_t = runner->id;

  }
  pthread_mutex_unlock(&timeTravel_signal_mutex);
}

void init_data(thread_data *thread){
  // Initialize Shared data
  create_map();
  shared_t.condition_t = 3;
  shared_t.goal_t = 0;
  shared_t.eliminated_t[0] = shared_t.eliminated_t[1] = shared_t.eliminated_t[2] = 0;
  shared_t.cycle_t = 0;
  shared_t.carrot_t[0][0] = shared_t.carrot_t[0][1] = shared_t.carrot_t[1][0] = shared_t.carrot_t[1][1] = 0;
  shared_t.carrot_holder_t[0] = shared_t.carrot_holder_t[1] = -1;
  shared_t.mtn_t[0] = shared_t.mtn_t[1] = 0;
  shared_t.exit_t = 0;

  // Initialize thread data
  thread[0].thread_id = 0;
  thread[0].id = 0;
  thread[0].condition = 3;
  thread[0].copy_goal = 0;
  thread[0].copy_cycle = 0;
  strcpy(thread[0].name, "Bunny");
  thread[0].letter = 'B';
  thread[0].carrot = 0;
  thread[0].x = 0;
  thread[0].y = 0;

  thread[1].thread_id = 1;
  thread[1].id = 1;
  thread[1].condition = 0;
  thread[1].copy_goal = 0;
  thread[1].copy_cycle = 0;
  strcpy(thread[1].name, "Taz");
  thread[1].letter = 'D';
  thread[1].carrot = 0;
  thread[1].x = 0;
  thread[1].y = 0;

  thread[2].thread_id = 2;
  thread[2].id = 2;
  thread[2].condition = 1;
  thread[2].copy_goal = 0;
  thread[2].copy_cycle = 0;
  strcpy(thread[2].name, "Tweety");
  thread[2].letter = 'T';
  thread[2].carrot = 0;
  thread[2].x = 0;
  thread[2].y = 0;

  thread[3].thread_id = 3;
  thread[3].id = 3;
  thread[3].condition = 2;
  thread[3].copy_goal = 0;
  thread[3].copy_cycle = 0;
  strcpy(thread[3].name, "Marvin");
  thread[3].letter = 'M';
  thread[3].carrot = 0;
  thread[3].x = 0;
  thread[3].y = 0;

  printf("threads have been created");
  // initialize time seed and random position
  setup_time_seed();
  init_pos(thread);
  print_map();
}


// API
void *run_API(void *thread){
  thread_data *runner = (thread_data*)thread;
  setup_time_seed();

  while(runner->copy_goal < 2){
    runner_signal(runner);
    sleep(2);
  }
  
  pthread_exit(NULL);

}

// Main
int main(int argc, char *argv[]){
  int i;
  thread_data thread[4];
  init_data(&thread);


  pthread_mutex_init(&timeTravel_signal_mutex, NULL);

  for(i = 0; i < 4; i++){
    thread[i].thread_id = i;
    pthread_create(&(thread[i].thread_id), NULL, run_API, (void *)(&thread[i]));
  }

  for(i = 0; i < 4; i++){
    pthread_join(thread[i].thread_id, NULL);
  }

  pthread_mutex_destroy(&timeTravel_signal_mutex);
  printf("Threads destroyed\n");
  // Print out winner
  printf("The winner is %s\n\n", shared_t.winner_t[0]);
  return 0;
}
