/* 
    File: dataserver.cpp

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 

    Dataserver main program for MPs in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "netreqchannel.hpp"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* VARIABLES */
/*--------------------------------------------------------------------------*/

static int nthreads = 0;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

void handle_process_loop(NetworkRequestChannel & _channel);

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

std::string int2string(int number) {
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- GENERATE THE DATA */
/*--------------------------------------------------------------------------*/

std::string generate_data() {
  // Generate the data to be returned to the client.
  usleep(1000 + (rand() % 5000));
  return int2string(rand() % 100);
}
/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- INDIVIDUAL REQUESTS */
/*--------------------------------------------------------------------------*/

void process_hello(NetworkRequestChannel & _channel, const string & _request) {
  _channel.cwrite("hello to you too");
}


void process_data(NetworkRequestChannel & _channel, const std::string &  _request) {
  std::string data = generate_data();
  _channel.cwrite(data);
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THE PROCESS REQUEST LOOP */
/*--------------------------------------------------------------------------*/

void process_request(NetworkRequestChannel & _channel, const std::string & _request) {

  if (_request.compare(0, 5, "hello") == 0) {
    process_hello(_channel, _request);
  }
  else if (_request.compare(0, 4, "data") == 0) {
    process_data(_channel, _request);
  }
  else {
    _channel.cwrite("unknown request");
  }

}

void handle_process_loop(NetworkRequestChannel & _channel) {

  for(;;) {

    std::cout << "Reading next request from channel "<< std::flush;
    std::string request = _channel.cread();
    std::cout << " done" << std::endl;
    std::cout << "New request is " << request << std::endl;

    if (request.compare("quit") == 0) {
      _channel.cwrite("bye");
      usleep(10000);       // give the other end a bit of time.
      break;                  // break out of the loop;
    }
    process_request(_channel, request);
  }
  _channel.~NetworkRequestChannel(); //close socket
}
/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- Connection Handler*/
/*--------------------------------------------------------------------------*/
void* connection_handler(int* fd){
  NetworkRequestChannel new_chan(*fd); //for the other functions to work I had to wrap the soket file descriptor with a request channel
  handle_process_loop(new_chan);
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
  //default parameters
	unsigned short port_number= 10101;
  int backlog = 10;
  int c = 0;
	while((c = getopt(argc, argv, "p:b:")) != -1){
		switch(c){
			case 'p':
				port_number = atoi(optarg);
				break;
			case 'b':
				backlog = atoi(optarg);
				break;
      case '?':
        cout<<"Only use this format: ./dataserver -p <port number for data server> -b <backlog of the server socket>";
		}
	}
	cout << "SERVER STARTED ON PORT: " << port_number << endl;
  NetworkRequestChannel control_channel(port_number,connection_handler, backlog);
  
}

