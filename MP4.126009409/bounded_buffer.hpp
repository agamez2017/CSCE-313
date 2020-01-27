/* 
    File: bounded_buffer.hpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science
            Texas A&M University
    Date  : 10/15/19
	

*/
#ifndef _bounded_buffer_H_        // include file only once
#define _bounded_buffer_H_

#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include "semaphore.cpp"
#include "mutex.cpp"
#include <string>
#include <queue>
#include <vector>

using namespace std;

class bounded_buffer{
	private:
			Mutex m;
			int N;
			Semaphore empty; //empty;
			Semaphore full; //full;
			queue<string> buff;
	public:
	//Constructor
	bounded_buffer(int N):empty(N), full(0){}; //use initialization list 
	//Destructor
	~bounded_buffer(){};
	//Produce function
	void Produce(string item);
	//Consume function
	string Consume();
};

#endif