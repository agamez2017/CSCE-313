/* 
    File: simpleclient.cpp

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2019/09/23

    Simple client main program for MP2 in CSCE 313
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

#include "reqchannel.hpp"

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

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

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

  for(int i = 0; i < 100; i++) {
    std::string request_string("data TestPerson" + int2string(i));
    std::string reply_string = chan.send_request(request_string);
    std::cout << "reply to request " << i << ":" << reply_string << std::endl;;
  }
 
  std::string reply4 = chan.send_request("quit");
  std::cout << "Reply to request 'quit' is '" << reply4 << std::endl;

  usleep(1000000);
}
