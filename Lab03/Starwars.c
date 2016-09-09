//Author: Saw Xue Zheng, Corey Zalewski

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structs
struct data_clone{
  int ID, timeCounter;
  char name[25];
};

// Function Header
int CreateClone(struct data_clone** warrior);
void InitClone(struct data_clone* clone, int ID, char* name, int timeCounter);
void Input(struct data_clone** warrior, int n_clone);
void AskId(struct data_clone* clone);
void AskName(struct data_clone* clone);
void AskTimeCounter(struct data_clone* clone);
void ClockDec(struct data_clone* clone);
bool LifeCheck(struct data_clone* clone);
void Print();
void LifeSpan();
void SWAPI(void);

// Function Implementation


//Description: Ask user to create a number of clones and input the details of the clones
int CreateClone(struct data_clone** warrior){

  int n_clone = 0;

  printf("Please input the number of clones to spawn: "); 
  scanf("%d", &n_clone);

  printf("Spawning %d clones...\n", n_clone);
  //points array to allocated memory
  *warrior = malloc(n_clone*sizeof(struct data_clone));

  if (warrior == NULL) { printf("Does not have enough memory for allocation. Exiting program\n"); exit(0);}


  return n_clone;
} //end CreateClone



//Description: Initialize clone with ID, Name and TimeCounter
void InitClone(struct data_clone* clone, int ID, char* name, int timeCounter)
{
  clone->ID = ID;
  strcpy(clone->name, name);
  clone->timeCounter = timeCounter;
}



//Description: Ask user for the information of clones and Initialize them
void Input(struct data_clone** warrior, int n_clone){

   for(int i = 0; i < n_clone; i++)
	{
		printf("Getting information for clone[%d]\n",i);
		
		/*
		AskId(&warrior[i]);
		AskName(&warrior[i]);
		AskTimeCounter(&warrior[i]);
		*/

		InitClone(&warrior[i],i, "Derp",5+(5*i) );
                printf("\n----Clone Information----\n");
		Print(&warrior[i], n_clone);
		printf("\n\n");
	}
}//end Input


//Description: Ask for ID of clone
void AskId(struct data_clone* clone){
  
  printf("Please input the clone id: "); 
  scanf("%i", &clone->ID);

  printf("Clone id is %i\n", clone->ID);//debug message


}//end AskId

//Description: Ask for Name of clone
void AskName(struct data_clone* clone){
  printf("Please input the clone name: "); 
  scanf("%s", clone->name);

  printf("Clone name is %s\n", clone->name); 

}//end Askname


//Description: Ask for time counter of clone
void AskTimeCounter(struct data_clone* clone){
  printf("Please input the time counter: "); 
  scanf("%d", &clone->timeCounter);

  printf("Clone timeCounter is %d\n", clone->timeCounter);//debug message 

}//end AskTimeCounter


void ClockDec(struct data_clone *clone){  // -- Corey
  clone->timeCounter--;
}

bool LifeCheck(struct data_clone *clone){  // -- Corey
  return clone->timeCounter > 0;
}

void Print(struct data_clone *clone, int alive){  // -- Corey
  if(alive == 0){
    printf("All clones are dead.\n");
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
        if(!LifeCheck(&clone[i])){	// Check if they are dead
          alive--;	// decrement the number of alive clones
        }
        else{
          ClockDec(&clone[i]);	// decrement life clock
          if(!LifeCheck(&clone[i])){ // if they died now
            alive--;  // decrement again
          }
        }
        Print(&clone[i], alive);	// Print out the clone
      }
    }
    clock++;	// Increment the time clock
  }while(alive != 0);
}

// API
void SWAPI(void){


  //Pointer to clones
  struct data_clone* warrior;

  //Create some clones
  int n_clone = CreateClone(&warrior);

  //Ask user for clone's info
  Input(warrior, n_clone);  //Right now is just initializing everythint to 'Derp'

  //Begin lifespan countdown
  //LifeSpan(warrior, n_clone);

  //Deallocate when exiting
  free(warrior);

}

int main(){
  // API
  SWAPI();

  return 0;
}
