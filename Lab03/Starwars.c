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

void Print(struct data_clone *clone){  // -- Corey
  
}

void LifeSpan(struct data_clone *clone, int n_clone){ //-- Corey
  int clock = 0, dead = 0;
}

// API
void SWAPI(void){

}

int main(){
  // API Call
  SWAPI();
  return 0;
}
