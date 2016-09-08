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

}

int main(){
  // API Call
  return 0;
}
