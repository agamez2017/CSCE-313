/* 
    File: free_list.cpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science
            Texas A&M University
    Date  : 9/8/2019

    Modified: 

    This file contains the implementation of the class FreeList.

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <iostream>
#include "free_list.hpp"

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
/* FUNCTIONS FOR CLASS SegmentHeader */
/*--------------------------------------------------------------------------*/

SegmentHeader::SegmentHeader(size_t _length, bool _is_free) {
  length = _length;
  is_free = _is_free;
  cookie = COOKIE_VALUE;
  // You may need to initialize more members here
  next = NULL;
  prev  = NULL;
  
}

SegmentHeader::~SegmentHeader() {
  // You may need to add code here.
}


void SegmentHeader::CheckValid() {
  if (cookie != COOKIE_VALUE) {
    cout << "INVALID SEGMENT HEADER!!" << endl;
    assert(false);
    // You will need to check with the debugger to see how we got into this
    // predicament.
  }
}

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS FreeList */
/*--------------------------------------------------------------------------*/

FreeList::FreeList() {
  header  = NULL;//initialize header to null pointer
  size=0;
}

FreeList::~FreeList() {
  //Piazza post: Leave empty as the destructor of myallocator takes care of it
}

bool FreeList::Add(SegmentHeader * _segment) {
	if(header == NULL)//if header is null then point to seg
	{
		header  = _segment;
		_segment->is_free = true;
		return true;
	}
	else //else just add to beginnig of freelist
	{
		_segment->next = header;
		header->prev = _segment;
		header = _segment;
		_segment->is_free =true;
		size++;
		return true;
	}
}

bool FreeList::Remove(SegmentHeader * _segment) {
	if(_segment == header)//if segment is header
	{
		header = _segment->next;
		if(_segment->next != NULL)
		{
			_segment->next->prev = NULL;
		}
		_segment->is_free = false;
		_segment->next = NULL;
		_segment->prev = NULL;
		size--;
		return true;
	}
	else if(_segment->prev != NULL && _segment->next ==NULL)//if segment is tail 
	{
		_segment->prev->next = NULL;
		_segment->prev= NULL;
		_segment->next = NULL;
		_segment->is_free = false;
		size--;
		return true;
	}
	else//segment is in between two segments
	{
		_segment->prev->next = _segment->next;
		_segment->next->prev = _segment->prev;
		_segment->prev = NULL;
		_segment->next =NULL;
		_segment->is_free = false;
		size--;
		return true;
	}
}


SegmentHeader* FreeList::split(SegmentHeader* cSeg, size_t l) {//split block and return new segment header to insert to free list
	SegmentHeader* newSeg = new((char*)cSeg+l) SegmentHeader(cSeg->length-l,true);
	cSeg->length = l;
	return newSeg;
}