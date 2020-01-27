/* 
    File: mutex.hpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science
            Texas A&M University
    Date  : 10/6/2019

	This file implements the functions of mutex.hpp
*/

#include "mutex.hpp"
#include<pthread.h>

//constructor
Mutex::Mutex()
{
	//Is it the  first one or the second one?
	//m = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&m,NULL);
}

//destructor
Mutex::~Mutex()
{
	pthread_mutex_destroy(&m);
}

//Lock()
void Mutex::Lock()
{
	pthread_mutex_lock(&m);
}

//Unlock()
void Mutex::Unlock()
{
	pthread_mutex_unlock(&m);
}

int main()
{
	
}