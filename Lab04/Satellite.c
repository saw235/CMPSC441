//Author: Saw Xue Zheng, Corey Zalewski

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Defines
#define totalCountries 5

// Notes
// countries {USA, China, Germany, Japan, Switerzerland}
// package {1, 2, 3, 4} Tera-byte
// price  {210, 350, 400, 500} $
// timeTransmission {1, 3, 5, 10} hour
// requestedTransmission {0, 1} off or on

// Global

// Structs
struct data_country{
  char name[15];
  int activated;
  int canTake;
  int selectedPack;
  // should we put the data_canTake int available in here?
}country[5];

struct data_queue{
  int country;
  int waiting;
}sequence[5];

struct data_channel{
  int countDown;
  int country;
}channel[2];

struct data_canTake{  // Note the comment in data_country
  int available;
};

// Function Headers
int getRandom(int, int);
void getActivate();
void chosenPack();
void canTake((struct data_country*, struct data_canTake*, int);
void sequence_queue();
void transmission_mode();
bool wait_countDown();
void waitToChannel(struct data_country &country, struct data_channel &channel, struct data_queue &sequence);
void popQueue(struct data_country &country, struct data_channel &channel, struct data_queue &sequence);
void SatelliteAPI();

// Function Declaration
int getRandom(int rangeLow, int rangeHigh){
  double myRand = randd()/(1.0 + RAND_MAX);
  int range = rangeHigh - rangeLow +1;
  int myRand_scaled = (myRand* range) + rangeLow;
  return MyRand_scaled;
}

// Description: Randomly sets if a country has requested to send transmission
void getActivate(){
  for(int i = 0; i < 5; i++){
    country[i].actived = getRandom(0,1);
  }
}

// Description: Randomly assign a package if they requested to transmit
void chosenPack(){
  for(int i = 0; i < 5; i++){
    country[i].selectedPack = getRandom(0, 3);
  }
}

// Description: 
void canTake(struct data_country *country, struct data_canTake *canTake, int totalCountries){
  int i;
  for(i = 0; i < totalCountries; i++){
    if(country[i].active){
      canTake[i]].available = country[i].active;
    }
    else{
      canTake[i].available = (int)0;
    }
  }
}

// Description: setup the queue for the satellite
void sequence_queue(){
  int i, j;
  bool taken;
  for(i = 0; i < 5; i++){
    taken = false;
    do{
      sequence[i].country =getRandom(0,4);
      for(j = 0; j < i; j++){
        if(sequence[i].country == sequence[j].country)
          taken = true;
      }
    }while(taken);
    sequence[i].waiting = country[sequence[i].country].canTake;
  }
}

// Description: Run throught what the satellite does
void transmission_mode(){
  while(wait_countDown()){
    for(i = 0; i < 2; i++){
      if(channel[i].countDown == 0)
        popQueue(country, channel[i], sequence);
      channel[i].countDown--;
    }
    hour++;
  }
}

// Description: Check if waiting and/or countdown?
bool wait_countDown(){
  bool running = false;
  int i;
  for(i = 0; i < 5; i++){
    if(sequence[i].waiting == 1)
      running = true;
  }
  for(i = 0; i < 2; i++){
    if(channel[i].countDown > 0)
      running = true;
  }
  return running;
}

// Description: Add a country in the queue to an available channel
void waitToChannel(struct data_country &country, struct data_channel &channel, struct data_queue &sequence){
  channel.country = sequence.country;
  switch(country[sequence.country].selectedPack){
    case 1: channel.countDown = 1; break;
    case 2: channel.countDown = 3; break;
    case 3: channel.countDown = 5; break;
    case 4: channel.countDown = 10; break;
    default: channel.countDown = 0; break;
  }
  sequence.waiting = 0;
}

// finds the next available country and push it to the open channel
void popQueue(struct data_country &country, struct data_channel &channel, struct data_queue &sequence){
  for(int i = 0; i < 5; i++){
    if(sequence[i].waiting == 1){
      waitToChannel(&country, &channel, &sequence[i]);
      break;
    }
  }
}

// API Declaration
void SatelliteAPI(){
  struct timeval time;
  gettimeofday(&time, NULL);
  
  getActivate();
  chosenPack();
  //canTake((struct data_country *country, struct data_canTake *canTake, int totalCountries);
  sequence_queue();
  transmission_mode();
}

int main(){
  // API Call
  SatelliteAPI();
  return 0;
}
