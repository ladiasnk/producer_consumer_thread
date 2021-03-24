/*
 *	File	: myFunctions.c
 *
 *	Description	: This file contains the definitions of the functions that are going
 *  to be used in the workFunction struct.
 *
 *	Author : Nikolaos Ladias
 *	Date  : March 2021
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "Functions.h"


//Definition of the workFuctions that will used in the queue.

void * fourthPower(void * arg){
  int number= (int) arg;
  signed long power=1;
  for(int i=1;i<=4;i++)
    power=power*number;
}

void * findCos(void * arg){
  int number;
  float myCos;
  number= (int) arg;
  for(int i=1;i<=FUNC_REPS;i++)
    myCos=cos((float)(  (i*number* PI) / 180) );
}

void * findSin(void * arg){
  int number;
  float mySin;
  number= (int) arg;
  for(int i=1;i<=FUNC_REPS;i++)
    mySin=sin((float)(  (i* number* PI) / 180) );
}

void * findCosSumSin(void * arg){
  int number;
  float mySin;
  float myCos;
  float mySum;
  number= (int) arg;
  mySin=sin((float)(  (number* PI) / 180)  );
  myCos=cos((float)(  (number* PI) / 180)  );
  mySum=mySin+myCos;
}

void * SqRoot(void * arg){
  int number;
  float myRoot;
  number= (int) arg;
  myRoot= sqrt((float)number);
}
