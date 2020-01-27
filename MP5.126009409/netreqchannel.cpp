/*
    File: netreqchannel.cpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science
            Texas A&M University
    Date  : 2019/11/19

    This file implements the class functions of NetworkRequestChannels.
*/
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include "netreqchannel.hpp"
#include <string.h>
#include <functional>
#include <thread>
using namespace std;

const int MAX_MESSAGE = 255; 

/***********************Class Constructor*********************************/
NetworkRequestChannel::NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no)
{
  string port = std::to_string(_port_no);
  struct addrinfo hints, *res;
	// load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int status;
	if ((status = getaddrinfo(_server_host_name.c_str(), port.c_str(), &hints, &res)) != 0) {
        cerr << "getaddrinfo: " << gai_strerror(status) << endl;
    }

	// make a socket
	fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  //error checking
	if (fd < 0)
	{
		perror ("Cannot create scoket");	
	}
	// connect
	if (connect(fd, res->ai_addr, res->ai_addrlen)<0)
	{
		perror ("Cannot Connect");
	}
};
/***********************Class Constructor**********************************/
NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, void * (*connection_handler) (int *),int backlog)
{
    //Server NetworkRequestChannel
		cout<<"Server started"<<endl;
		int optval = 1;
		struct sockaddr_in addr;
    //set socket
		fd = socket(AF_INET,SOCK_STREAM,0);
    //error checking
		assert(fd > -1);
		setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(const void *) &optval,sizeof(int));
		bzero((char*) &addr,sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(_port_no);
		//bind
		int rc = bind(fd,(struct sockaddr*) &addr,sizeof(addr));
		assert(rc == 0);
		//listen
		rc = listen(fd,backlog);
		assert(rc == 0);
		//accept new connections
		struct sockaddr_in client;
    socklen_t client_size;
		while(1) { 
          cout<<"About to find a connection\n";
        	int new_connection = accept(fd, (struct sockaddr *)&client,&client_size);
        	if (new_connection == -1) {
            	perror("SERVER: Accept error.");
            	exit(-1);
        	}
          std::function<void*(int*)> new_f(connection_handler);
          cout<<"Found Connection\n";
        	std::thread t(new_f,&new_connection);
          t.detach();
    }
};

NetworkRequestChannel::NetworkRequestChannel(const int sock)
{
  fd = sock;
}

/************************Class Destructor**********************************/
NetworkRequestChannel::~NetworkRequestChannel()
{
  close(fd);
};

/*************************Class functions***********************************/
string NetworkRequestChannel::send_request(string _request)
  /* Send a string over the channel and wait for a reply. */
{
  cwrite(_request);
  std::string s = cread();
  return s;
};

string NetworkRequestChannel::cread()
  /* Blocking read of data from the channel. Returns a string of characters
     read from the channel. Returns NULL if read failed. */
{
    char buf[MAX_MESSAGE];

    if (read(fd, buf, MAX_MESSAGE) < 0) 
    {
       perror(std::string("Request Channel, Error reading from pipe!").c_str());
        return NULL;
    }
    std::string s = buf;
    return s;
};

int NetworkRequestChannel::cwrite(string _msg)
  /* Write the data to the channel. The function returns the number of characters written
     to the channel. */
{
    if (_msg.length() >= MAX_MESSAGE) 
    {
        std::cerr << "Message too long for Channel!" << std::endl;
    }
    const char * s = _msg.c_str(); // NOTE: c_str() is NOT thread safe!!
    if (write(fd, s, strlen(s)+1) < 0) 
    {
        perror(std::string("Request Channel, Error writing to pipe!").c_str());
    }
    return strlen(_msg.c_str());
};

int NetworkRequestChannel::read_fd()
{
  return fd;
}

