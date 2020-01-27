/* 
    File: client.cpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science
            Texas A&M University
    Date  : 10/6/2019
	

    Client main program for MP2 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include<assert.h>
#include "reqchannel.hpp"
#include <ctime>
#include <chrono>

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

std::string int2string(int number) {
  std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}


//Used for task 2, calling the function locally
std::string generate_data() {
  // Generate the data to be returned to the client.
  return int2string(rand() % 100);
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  int pid = fork();
  if(pid==0) //if child execute the dataserver 
  {
	  execl("dataserver", (char*) NULL);
  }
  else{
	  
  auto start=chrono::high_resolution_clock::now();
  std::cout << "CLIENT STARTED:" << std::endl;

  std::cout << "Establishing control channel... " << std::flush;
  RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
  std::cout << "done." << std::endl;

  /* -- Start sending a sequence of requests */
  
  std::string reply1 = chan.send_request("hello");
  std::cout << "Reply to request 'hello' is '" << reply1 << "'" << std::endl;

  std::string reply2 = chan.send_request("data Joe Smith");
  std::cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << std::endl;

  std::string reply3 = chan.send_request("data Jane Smith");
  std::cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << std::endl;

  int num_of_patients = 100000;
  for(int i = 0; i < num_of_patients; i++) {
    std::string request_string("data TestPerson" + int2string(i));
    std::string reply_string = chan.send_request(request_string);
    std::cout << "reply to request " << i << ":" << reply_string << std::endl;;
  }
 
  std::string reply4 = chan.send_request("quit");
  std::cout << "Reply to request 'quit' is '" << reply4 << std::endl;
  //Following two lines used for timing puposes.
  auto finish=chrono::high_resolution_clock::now();
  cout<<"Time: "<<((double)chrono::duration_cast<chrono::milliseconds>(finish-start).count())/1000<<" seconds"<<endl;
  
  usleep(1000000);
  
  
 /*
  //TASK 2 Purpose to call generate_data locally 
  auto start=chrono::high_resolution_clock::now();
    int num_of_patients = 1000000;

  struct timeval tp_start2; // Used to compute elapsed time. 
  struct timeval tp_end2;
  assert(gettimeofday(&tp_start2, 0) == 0);
  for(int i =0;i<num_of_patients;i++)
  {
	  cout<<generate_data()<<endl;
  }
   auto finish=chrono::high_resolution_clock::now();
   cout<<"Time: "<<((double)chrono::duration_cast<chrono::milliseconds>(finish-start).count())/1000<<" seconds"<<endl;
  usleep(1000000);*/
  
  }
}
