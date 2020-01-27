/* 
    File: mutex_guard.cpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science
            Texas A&M University
    Date  : 10/6/2019
	
	This file implements functions in mutex_guard.hpp
*/

#include "mutex_guard.hpp"
#include<pthread.h>


//Constructor
MutexGuard::MutexGuard(Mutex &m)
{
	m.Lock();
}
//Destructor
MutexGuard::~MutexGuard()
{
	m->Unlock();
}

int main()
{
	
}