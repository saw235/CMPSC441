#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // exit :: may be needed on CodeBlocks;

#define NUM_THREADS     3

struct thread_data{
	char *message;
	int  thread_id;
}thread_data_array[NUM_THREADS]={
        {"Hello World",(int)0},
        {"extremely happy",(int)0},
        {"Walking on the beach at sunset",(int)0}
                };

void *PrintHello(void *threadarg)
{  long tid;
   struct thread_data *my_data;
   my_data = (struct thread_data *) threadarg;
   int taskid = my_data->thread_id;
   char *hello_msg = my_data->message;

   printf("||thread #%d || %s\n ", my_data->thread_id,my_data->message);
   my_data->thread_id+=10;
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{  pthread_t threads[NUM_THREADS];
   int error;
   long t;

   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      thread_data_array[t].thread_id = t;

      error = pthread_create(&threads[t], NULL, PrintHello, (void *) &thread_data_array[t]);
      if (error){
         printf("ERROR; return code from pthread_create() is %d\n", error);
         exit(-1);
      }
   }

   for(t=0; t<NUM_THREADS; t++){
	   printf("value Thread +10 = %d\n",thread_data_array[t].thread_id);
   }

   /* for the case main finishing before some thread */
   pthread_exit(NULL);

   return 0;
}

