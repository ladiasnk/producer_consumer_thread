/*
 *	File	: Functions.h
 *
 *	Description	: This file contains the declarations of the functions that are going
 *  to be used in the workFunction struct.
 *
 *	Author : Nikolaos Ladias
 *	Date  : March 2021
 *
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define PI 3.141592654
#define N_OF_FUNCTIONS 5
#define FUNC_REPS 10

/* Declaration of the workFuctions that consist the fuctions array, their definition
is in the myFunctions.c file*/

//This function calculates the 5th power of the arg number by multiplying it with itself.
void * fourthPower(void * arg);

//This function calculates 10 cosines staring with the args' one.
void * findCos(void * arg);

//This function calculates 10 sines staring with the args' one.
void * findSin(void * arg);

//This function calculates the sum of argument's sine and cosine and prints it.
void * findCosSumSin(void * arg);

//This function calculates argument's square root and prints it.
void * SqRoot(void * arg);





#endif
