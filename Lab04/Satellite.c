//Author: Saw Xue Zheng, Corey Zalewski

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

const int totalCountries = 5;
int hour = 0;
int totalProfit = 0;

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
  int selectedPack;
};

struct data_queue{
  int country;
  int waiting;
};

struct data_channel{
  int countDown;
  int country;
};

struct data_canTake{
  int available;
};

// Function Headers
int getRandom(int, int);
void getActivate(struct data_country*);
void chosenPack(struct data_country*);
void CanTake(struct data_country*, struct data_canTake*);
void sequence_queue(struct data_queue*, struct data_country*);
void transmission_mode(struct data_channel*, struct data_country*, struct data_queue*);
bool wait_countDown(struct data_queue*, struct data_channel*);
void waitToChannel(struct data_country *country, struct data_channel *channel, struct data_queue *sequence);
void popQueue(struct data_country *country, struct data_channel *channel, struct data_queue *sequence);
void profit(struct data_country *country);
void initPrint(struct data_country *country, struct data_queue *sequence);
void transmissionPrint(struct data_channel *channel, struct data_country *country);
void finalPrint();
void SatelliteAPI();

// Function Declaration
int getRandom(int rangeLow, int rangeHigh) {
  double myRand = rand() / (1.0 + RAND_MAX);
  int range = rangeHigh - rangeLow + 1;
  int myRand_scaled = (myRand * range) + rangeLow;
  return myRand_scaled;
} // end getRandom()


// Description: Randomly sets if a country has requested to send transmission
void getActivate(struct data_country* country){
  for(int i = 0; i < 5; i++){
    country[i].activated = getRandom(0,1);
  }
  printf("Activated successfully\n");
}

// Description: Randomly assign a package if they requested to transmit
void chosenPack(struct data_country* country){
  for(int i = 0; i < 5; i++){
    if(country[i].activated == 1)
      country[i].selectedPack = getRandom(0, 3);
    else
      country[i].selectedPack = -1;
  }
  printf("Package selected successfully\n");
}

// Description: 
void CanTake(struct data_country *country, struct data_canTake *canTake){
  int i;
  for(i = 0; i < totalCountries; i++){
    if(country[i].activated){
      canTake[i].available = country[i].activated;
    }
    else{
      canTake[i].available = (int)0;
    }
  }
  printf("Can Take successfully\n");
}

// Description: setup the queue for the satellite
void sequence_queue(struct data_queue* sequence, struct data_country* country){
  int i, j;
  bool taken;
  for(i = 0; i < 5; i++){
	// Possible rewrite to use the data_canTake to check
    do{
      taken = false;
      sequence[i].country = getRandom(0,4);
      for(j = 0; j < i; j++){
        if(sequence[i].country == sequence[j].country)
          taken = true;
      }
    }while(taken == true);
    sequence[i].waiting = country[sequence[i].country].activated;
  }
  printf("Queue has been built\n");
}

// Description: Run through what the satellite does
void transmission_mode(struct data_channel * channel, struct data_country* country, struct data_queue* sequence){
  int i;
  while(wait_countDown(sequence, channel)){
    for(i = 0; i < 2; i++){
      if(channel[i].countDown == 0)
        popQueue(country, &channel[i], sequence);
      channel[i].countDown--;
    }
    hour++;
    transmissionPrint(channel, country);
  }
}

// Description: Check if waiting and/or countdown?
bool wait_countDown(struct data_queue* sequence, struct data_channel* channel){
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
void waitToChannel(struct data_country *country, struct data_channel *channel, struct data_queue *sequence){
  channel->country = sequence->country;
  switch(country[sequence->country].selectedPack + 1){
    case 1: channel->countDown = 1; break;
    case 2: channel->countDown = 3; break;
    case 3: channel->countDown = 5; break;
    case 4: channel->countDown = 10; break;
    default: channel->countDown = 0; break;
  }
  sequence->waiting = 0;
}

// finds the next available country and push it to the open channel
void popQueue(struct data_country *country, struct data_channel *channel, struct data_queue *sequence){
  for(int i = 0; i < 5; i++){
    if(sequence[i].waiting == 1){
      waitToChannel(&country[i], channel, &sequence[i]);
      break;
    }
  }
}

// Description: Calculate the amount of money made by transmitting
void profit(struct data_country *country){
  for(int i = 0; i < 5; i++){
    switch(country[i].selectedPack + 1){
      case 1: totalProfit += 210; break;
      case 2: totalProfit += 350; break;
      case 3: totalProfit += 400; break;
      case 4: totalProfit += 500; break;
      default: totalProfit += 0; break;
    }
  }
}



void initPrint(struct data_country *country, struct data_queue *sequence){
  int i;
  for(i = 0; i < 5; i++){
    printf("Country: 	%s\nActivated:	%d\nPackage:	%d TB\n\n", country[i].name, country[i].activated, country[i].selectedPack + 1);
  }
  printf("Satellite Queue:\n");
  for(i = 0; i < 5; i++){
    if(sequence[i].waiting == 1)
      printf("%s\n",country[sequence[i].country].name);
  }
}


void transmissionPrint(struct data_channel *channel, struct data_country *country){
  printf("-------------------\nHour:	%d\n", hour);
  for(int i = 0; i < 2; i++){
    printf("\nChannel:	%d\nCountry:	%s\nTime Left:	%d hr\n", i + 1, country[channel[i].country].name, channel[i].countDown);
  }
  printf("-------------------\n\n");
}


void finalPrint(){
  printf("Satellite going into sleepmode for maintenance\nTotal Hours: %d\nTotal Profit: $%d\n", hour, totalProfit);
}

// API Declaration
void SatelliteAPI(){
  srand(time(NULL));

  // Initialize structs
  struct data_country country[5] = {{"USA", 0, 0},{"China", 0, 0}, {"Germany", 0, 0},{"Japan", 0, 0}, {"Switerzerland", 0, 0}};
  struct data_channel channel[2] = {{0, 0}, {0, 0}};
  struct data_queue sequence[5];
  struct data_canTake canTake[5];	// I think we need to use this for the queueing part

  getActivate(&country);
  chosenPack(&country);
  CanTake(&country, &canTake);
  sequence_queue(&sequence, &country);
  initPrint(&country, &sequence);
  transmission_mode(&channel, &country, &sequence);
  profit(&country);
  finalPrint();
}

int main(){
  // API Call
  SatelliteAPI();
  return 0;
}
