/*
 *	File	: producer-consumer_modified.c
 *
 *	Description	: Modify the given pthreads based solution to the producer consumer problem using
 *  multiple consumer and producer threads, and using thread functions as a FIFO
 *  queue elements.
 *
 *	Author : Nikolaos Ladias
 *	Date  : March 2021
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "Functions.h"
#define QUEUESIZE 10
#define LOOP 10000
#define N_OF_ARGS 10
#define N_OF_FUNCTIONS 5
int P; // number of producer Threads
int Q; // number of consumer Threads

//declare file pointer to store all final execution times
FILE *allExecutionTimes;

//Given struct for defining the workFuction our thread will execute.
struct workFunction {
  void * (*work)(void *);
  void * arg;
};



//Consumer and Producers Threads functions declaration.
void *producer (void *args);
void *consumer (void *args);



//Initialization of the workFunctions' fuctions array
void * (* functions[N_OF_FUNCTIONS])(void *)= {&fourthPower,&findCos ,&findSin,&findCosSumSin,&SqRoot};

//Initialization of the workFunctions' arguments array(just some random numbers )
int arguments[N_OF_ARGS]={ 2    , 10   , 25   , 60 ,70 ,
                    150 , 500   , 10000   ,50000,100000 };


/*The startingTimes array, is equally sized with our queue.This basically helps us calculate the
waiting time of a workFunction in the queue.When a new item is stored in a queue position (index),
 the time is stored in the same position in the startingTimesArray . For this we need a strcut variable of type timeval*/
struct timeval startingTimes[QUEUESIZE] ;

/*we need time in nanoconds for bigger precision(nanosecond precision) where the execution time is
small (for instance 0.8 usec). Generaly we store the measurements from gettimeofday as we are asked,
but when waiting time is less than 5 microseconds we store the measurement fron clock_gettime
convereted in microseconds (usec). For this, we need a struct variable of type timespec*/
struct timespec startingTimes2[QUEUESIZE] ; //getting time is nanoseconds


//Struct defined for the queue implementation
typedef struct {
  struct workFunction buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;


//queue methods (functions) definition
queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, struct workFunction in);
void queueExecution ( queue *q,struct workFunction  workFunc,int currHead);

//variable that holds the number of total fuctions' executions by the consumers
long functionsCounter ;

//variable that hold the mean waiting-time of a function in the queue
double meanWaitingTime;

//variable that helps get the termination condition
int finishStatus;

int main (int argc, char* argv[])
{


  //pass the arguments of main for P and Q number of threads
  P=atoi(argv[1]);
  Q=atoi(argv[2]);


  //Getting results in files
  //declaring files' pointers for mean , max , min and the text file for console prints
  FILE  *MeanWaitingTimes;


  char filename[sizeof "ExecTimesPXXX_QXXX.csv"];

  //Giving to the file the proper name
  sprintf(filename, "ExecTimesP%03d_Q%03d.csv", P,Q);

  //Open the file where all the function waiting times of the current execution are stored
  allExecutionTimes = fopen(filename,"a");



  queue *fifo; //queue declaration
  pthread_t producers[P], consumers[Q];//threads declaration

  //Initializing to zero the two global variables for the mean waiting-time calculations .
  functionsCounter=0;
  meanWaitingTime=0;
  finishStatus=0;
  fifo = queueInit (); //queue initialization
  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
 /* First spawn the consumer threads, in order to be able to execute functions
  as soon as queue is not empty , with the help of condition variables and mutexes*/
 
  for (int i = 0; i < Q; i++)
    if(pthread_create (&consumers[i], NULL, consumer, fifo)!=0)
      perror("Failed to create thread!");
      // message to print if the creation of any thread fails.
  //producers threads spawning
    for (int i = 0; i < P; i++)
        if(pthread_create(&producers[i], NULL, producer, fifo)!=0)
        perror("Failed to create thread") ;
            // message to print if the creation of any thread fails.
     
      //join producers threads
    for (int i = 0; i < P; i++)
        if(pthread_join(producers[i], NULL)!=0) 
        perror("Failed to create thread");
         // message to print if the creation of any thread fails.
      //join consumers threads
    for (int i = 0; i < Q; i++)
        if(pthread_join(consumers[i], NULL)!=0)
        perror("Failed to create thread");
            // message to print if the creation of any thread fails.

// now the queue is deleted and the file that holds mean waiting time is opened and the data is written in it,along with all the execution times in a text file, then the files are closed

  //queue deletion
  queueDelete (fifo);
  // message to announce the program finished execution
  printf("\nPROGRAMM EXECUTION FINISHED. \n");

  //announce Producers and Q(consumers) threads number and the mean waiting time of the function waiting time in queue 
  printf("For P=%d, and Q=%d ,QUEUESIZE=%d the mean waiting-time is : %lf usec \n \n",P,Q,QUEUESIZE,meanWaitingTime);


  //open the files that store mean , min and max waiting time of all executions
  MeanWaitingTimes=fopen("dataΜΕΑΝ.csv","a");
 
  //printing to our file the mean waiting time of the executions
  fprintf(MeanWaitingTimes,"%d,%d,%lf\n",P,Q,meanWaitingTime);
  fclose(allExecutionTimes);
  return 0;
}

//producers' threads function
void *producer (void *q)
{
  queue *fifo; //pointer to a queue struct item
  int argIndex,funcIndex;

  fifo = (queue *)q;

  //producers' loop
  for (int i = 0; i < LOOP; i++)
  {
    pthread_mutex_lock (fifo->mut);

    while (fifo->full) {
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }

    // Declaring the workFuction that is going to be added in the queue.
    struct workFunction  work_func;

    //getting the workFuctions' function and argument randomly.
    argIndex=rand() % N_OF_ARGS;
    funcIndex=rand() % N_OF_FUNCTIONS;

    //assign the function and argument to our struct created
    work_func.work = functions[funcIndex];

    work_func.arg = (void *) arguments[argIndex];

    //the function that adds a test function in the queue
    queueAdd (fifo, work_func);
   
    //Check if we are in the last rep of producer
    if(i< LOOP-1){
      pthread_mutex_unlock (fifo->mut); //unlocking
      pthread_cond_signal (fifo->notEmpty); //signal in case cons thread is blocked
    }
    /*If we are in the last repetion of a producer in the loop, we update the finishStatus value.
    If finishStatus now is equal to P (producers' number), then all producers finished
    adding functions in the queue. */
    else{
      finishStatus++; //updating finishStatus
      pthread_mutex_unlock (fifo->mut);
      pthread_cond_signal (fifo->notEmpty);
    }
  }

  return (NULL);
}

//consumers' threads function
void *consumer (void *q)
{
  queue *fifo;
  fifo = (queue *)q;

  while (1) {
   //we first need to lock the mutex so that consumer can act on it undisturbed
    pthread_mutex_lock (fifo->mut);
    /*When all the producers finished adding new workFunction items in the queue (finishStatus==P)
     and the fifo queue is empty , the termination condition is met and the
     consumer Threads return.*/
    
    while (fifo->empty ) {

      //Termination condition,finishStatus ==P means that all producers finished adding workFuncs in the queue.
      if(finishStatus ==P){

        //Unlock the mutex before termination in order all the producers threads to terminate.
        pthread_mutex_unlock (fifo->mut);
        pthread_cond_signal (fifo->notEmpty);
        return NULL;
      }

      pthread_cond_wait (fifo->notEmpty, fifo->mut);

    }
    //variable to store the function the producer thread will execute, after unlocking
    struct workFunction execFunc;
    //The head value that corresponds to the queue position of the workFunction thread will execute
    long currHead;

    currHead=fifo->head;// assign the value to currHead by assigning the value head to the pointer fifo 
    execFunc=fifo->buf[currHead];// same here except that now we assign the workFunction
   //ready to pass the arguments to our queuExecution function
    queueExecution (fifo,execFunc,currHead);


  }
  return (NULL);
}

//Queue Initialization Function
queue *queueInit (void)
{
  queue *q;

  q = (queue *)malloc (sizeof (queue));
  if (q == NULL) return (NULL);

  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;
  q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (q->mut, NULL);
  q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notFull, NULL);
  q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notEmpty, NULL);

  return (q);
}

//Queue Destroy Function
void queueDelete (queue *q)
{
  pthread_mutex_destroy (q->mut);
  free (q->mut);
  pthread_cond_destroy (q->notFull);
  free (q->notFull);
  pthread_cond_destroy (q->notEmpty);
  free (q->notEmpty);
  free (q);
}

//Method that adds a new workFunction in the queue
void queueAdd (queue *q, struct workFunction in)
{
  //A new workFunction is added in the queue
  q->buf[q->tail] = in;

  // THE BEGINNING of the WAITING TIME is after the workFunction is added in the queue.
  gettimeofday(&startingTimes[q->tail],NULL);
  clock_gettime(CLOCK_MONOTONIC, &startingTimes2[q->tail]);

  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

  return;
}

void queueExecution ( queue *q,struct workFunction  workFunc,int currHead)
{
  ////TIME CALCULATIONS/////
  //variable to store the waiting time of the current workFunc
  long currWaitingTime =0 ;
  long currWaitingTime2=0 ;
  //variable to get the time that workFunction is getting out of the queue( before execution)
  struct timeval endTime;
  struct timespec endTime2;
  //The END of the waiting time , is the moment exactly before the function is executed.
  gettimeofday(&endTime,NULL);
  clock_gettime(CLOCK_MONOTONIC, &endTime2);
  //calculating waiting time in microseconds.
  currWaitingTime= (endTime.tv_sec*1e6 -(startingTimes[currHead] ).tv_sec*1e6);
  currWaitingTime+= (endTime.tv_usec-(startingTimes[currHead] ).tv_usec);
  //calculating waiting time in nanoseconds.
  currWaitingTime2=(endTime2.tv_sec-(startingTimes2[currHead ]).tv_sec) * 1e9  ;
  currWaitingTime2+=(endTime2.tv_nsec-(startingTimes2[currHead ]).tv_nsec  );
  if(currWaitingTime>=5)
    fprintf(allExecTimes,"%ld\n ",currWaitingTime);
  else
    //for small waiting times we use nanoseconds precision
    fprintf(allExecTimes, "%lf\n", ((float)currWaitingTime2)/1000);
  //updating global variables that are used for calculating the mean waiting time.
  ++functionsCounter;
  //Updating Head Value for the next consumer thread,before unlocking the mutex
  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;
  //Updating the mean waiting time of a function value
  meanWaitingTime= (meanWaitingTime*(functionsCounter-1) + (double)currWaitingTime )/(functionsCounter) ;
  //Unlocking the mutex
  pthread_mutex_unlock (q->mut);
  pthread_cond_signal (q->notFull);
  /*Executing the workFunction function after unlocking the mutex , this leads to parallel
  execution of workFunction functions*/
  (workFunc.work)((workFunc.arg));
  return;

}
