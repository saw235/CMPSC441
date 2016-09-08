//Author: Saw Xue Zheng, Corey Zalewski

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
int CreateClone(struct data_clone* warrior);
void Input();
void ClockDec(struct data_clone clone);
bool LifeCheck(struct data_clone clone);
void Print();
void LifeSpan();
void SWAPI(void);

// Function Implementation


//Description: Ask user to create a number of clones and input the details of the clones
int CreateClone(struct data_clone* warrior){

  int n_clone = 0;

  printf("Please input the number of clones to spawn: "); 
  scanf("%d", &n_clone);

  printf("Spawning %d clones...\n", n_clone);
  warrior = (struct data_clone*) malloc(n_clone*sizeof(struct data_clone));

  return n_clone;
}

void Input(){

}

void ClockDec(struct data_clone clone){
  clone.timeCounter--;
}

bool LifeCheck(struct data_clone clone){
  return clone.timeCounter > 0;
}

void Print(){

}

void LifeSpan(){

}

// API
void SWAPI(void){


  //Pointer to clones
  struct data_clone* warrior;

  //Create some clones
  int n_clone = CreateClone(warrior);

  //Begin lifespan countdown
  LifeSpan();


  //Deallocate when exiting
  free(warrior);

}

int main(){
  // API Call
  
  struct data_clone* warrior;
  int n_clone = CreateClone(warrior);
  
  free(warrior);
  return 0;
}
