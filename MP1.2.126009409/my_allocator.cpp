/* 
    File: my_allocator.cpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science
            Texas A&M University
    Date  : 9/8/2019

    Modified: 

    This file contains the implementation of the class MyAllocator.
	Sources:Piazza, TA in lab
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include "my_allocator.hpp"
#include <assert.h>
#include <iostream>

/*--------------------------------------------------------------------------*/
/* NAME SPACES */ 
/*--------------------------------------------------------------------------*/

using namespace std;
/* I know, it's a bad habit, but this is a tiny program anyway... */

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
/* FUNCTIONS FOR CLASS MyAllocator */
/*--------------------------------------------------------------------------*/

MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size) {
  orig_start = (char*) (std::malloc(_size)); //points to the memory block allocated
  SegmentHeader* nSeg = new(orig_start) SegmentHeader(_size, true);
  free_list.Add(nSeg);
  bbs = _basic_block_size;
  size= _size;
}

MyAllocator::~MyAllocator() {
  std::free(orig_start); 		//free memory block 
}

Addr MyAllocator::Malloc(size_t _length) {
	//calculate _length +sizeof(SegmentHeader) rounded up to basic block size
	//get segment 
	//flist.remove(currSegmentHeader)
	//flist.split()
	//flist.add(nseg)
	//return (char*) currSeg+sizeof(SegmentHeader)
	int reqlen = 0;
	int numtoround = _length+sizeof(SegmentHeader);
	int remainder = numtoround % bbs;
	if(remainder == 0)
	{
		reqlen = numtoround;
	}
	else
	{
		reqlen = numtoround+bbs-remainder;
	}
	SegmentHeader* currSeg;
	SegmentHeader* temp = free_list.getHeader();
	while(temp!= NULL)
	{
		if(temp->length >= reqlen)
		{
			currSeg = temp;
			break;
		}
		temp=temp->next;
		cout<<"got here\n";
	}
	if(currSeg == temp)
	{
		free_list.Remove(currSeg);
		SegmentHeader* nSeg = free_list.split(currSeg, reqlen);
		free_list.Add(nSeg);
		return (char*)currSeg+ sizeof(SegmentHeader);
	}
	else
	{
		return nullptr;
	}
}

bool MyAllocator::Free(Addr _a) {
  SegmentHeader* start = (SegmentHeader*)((char*)_a-sizeof(SegmentHeader));
  start->is_free = true;
  free_list.Add(start);
  return true;
}

