#Author: Saw Xue Zheng, Corey Zalewski

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structs
struct data_clone{
  int ID, timeCounter;
  char* name[25];
};

// Function Header
void CreateClone();
void Input();
void ClockDec(struct data_clone clone);
bool LifeCheck(struct data_clone clone);
void Print();
void LifeSpan();
void SWAPI(void);

// Function Implementation
void CreateClone(){

}

void Input(){

}

void ClockDec(struct data_clone *clone){  // -- Corey
  clone->timeCounter--;
}

bool LifeCheck(struct data_clone *clone){  // -- Corey
  return clone->timeCounter > 0;
}

void Print(struct data_clone *clone, int alive){  // -- Corey
  if(alive == 0){
    printf("All clones are dead");
  }
  else{
    printf("ID: %i\nName: %s\nTime Counter: %i\n", clone->ID, clone->name, clone->timeCounter);
    if(LifeCheck(clone)){
      printf("Clone is alive\n");
    }
    else{
      printf("Clone is DEAD!!!\n");
    }
  }
}

void LifeSpan(struct data_clone *clone, int n_clone){ //-- Corey
  int clock = 0, alive = n_clone;
  do{
    printf("Time: %i\n", clock);
    if(alive != 0){	// Make sure all the clones are not dead
      for(int i = 0; i < n_clone; i++){
        if(!LifeCheck(clone[i]){	// Check if they are dead
          alive--;	// decrement the number of alive clones
        }
        else{
          ClockDec(clone[i]);	// decrement life clock
          if(!LifeCheck(clone[i]){ // if they died now
            alive--;  // decrement again
          }
        }
        Print(clone[i], alive);	// Print out the clone
      }
    }
    clock++;	// Increment the time clock
  }while(alive != 0);
}

// API
void SWAPI(void){

}

int main(){
  // API Call
  SWAPI();
  return 0;
}
