/* 
    File: my_allocator.cpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science
            Texas A&M University
    Date  : 9/29/2019

    Modified: 

    This file contains the implementation of the class MyAllocator.
	Sources:Piazza, TA in lab
	Note: majority of functions are here.
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
#include <cmath>

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
  bbs = _basic_block_size;// basic bloack size
  mem_size = _size;//total memory 
  cout<<"BBS:"<<bbs<<endl;
  cout<<"mem_size:"<<mem_size<<endl;
  Create_v_of_flist(_size);//creates the vector of freelist 
  size_t real_size = v_of_flists.back().get_Fib_num()*bbs;//actual size of memory we are allocating in multiples of bbs and fibonacci numbers
  total_memory_length = real_size;
  endofMemory = orig_start+real_size;
  SegmentHeader* nSeg = new (orig_start) SegmentHeader(real_size,true,false,true);//add the first segment, with the corrected size
  v_of_flists.back().Add(nSeg); // we know that the last list is the biggest so we add the new seg to that last list
  cout<<"Size of Vector of Freelists: "<<v_of_flists.size()<<endl;
  cout<<"Start Address: "<<(void*)orig_start<<endl;
  cout<<"End Address: "<<(void*)endofMemory<<endl;
}

MyAllocator::~MyAllocator() {
  std::free(orig_start); 		//free entire memory block 
}

Addr MyAllocator::Malloc(size_t _length) {
	int indexoflist=-1;
	cout<<"Bytes asked for in malloc(): "<<_length<<endl;
	uint min = roundtoNearestFibsize(_length);
	cout<<"Minimum memory allocated: "<<min<<endl;
	if(min>mem_size)//if the call to malloc() exceeds what we have then its a problem.
	{
		cout<<"Requested memory exceeds total memory length.\n";
		return nullptr;
	}
	bool loopStop = false; //flag for the while loop
	SegmentHeader* foundSeg = nullptr;
	while(!loopStop) //this while loop allows splitting up blocks until we get a perfect size block or we don't have any left 
	{
		for(int i=0; i<v_of_flists.size(); i++)//for loop to traverse the vector of freelists
		{
			foundSeg = v_of_flists[i].getHeader();
			if(foundSeg != nullptr && foundSeg->length==min)//we found the perfect sized segment header
			{
				loopStop = true;
				indexoflist=i;
				cout<<"Returned block of size:"<<v_of_flists[i].get_Fib_num()*bbs<<endl;
				v_of_flists[i].Remove(foundSeg);
				return ((char*)foundSeg)+sizeof(SegmentHeader);
			}
			if(foundSeg!= nullptr&& foundSeg->length>min)
			{
				loopStop = false;//change back to false;
				indexoflist =i;
				if(indexoflist >=2)//only split until 3 which is index 2
					{
						cout<<"Found block size of :"<<v_of_flists[i].get_Fib_num()*bbs<<endl;
						Split(foundSeg,indexoflist);//split block and add them back to the freelist
						break; //break the for loop and start over.
					}
			}
		}
		if(foundSeg == nullptr && indexoflist== -1) // if no more free block then return nullptr
		{
			loopStop = true;
			cout<<"No more free blocks found.\n";
			return nullptr;
		}
	}
}

bool MyAllocator::Free(Addr _a) {//something wrong here
	cout<<"Freeing:\n";
	if (_a == NULL) 
	{
		return false;
	}
	SegmentHeader* start = (SegmentHeader*)((char*)_a-sizeof(SegmentHeader));
	start->CheckValid();
	//cout<<"Is segment being freed is valid: "<<valid<<endl;
	start->is_free = true;
	cout<<"Segment size being freed: "<<start->length<<endl;
	int index=findIndex(start);
	cout<<"Index of segment being freed: "<<index<<endl;
	SegmentHeader* bud = nullptr;
	bool loopstop = false;
	while(!loopstop)
	{
		bud = getBuddy(start);
		if(bud->is_free==true && ((char*)bud)<endofMemory && ((char*)bud)>=orig_start)
		{
			if(bud->is_left==true)
			{
				cout<<"start: "<<(void*)start<<endl;
				cout<<"start is left: "<<start->is_left<<endl;
				cout<<"Bud : "<<(void*)bud<<endl;
				cout<<"BUd is left: "<<bud->is_left<<endl;
				Merge(bud, start);
				return true;
			}
			if(bud->is_left==false)
			{
				Merge(start,bud);
				return true;
			}
		}
		else if(bud->is_free!=true)
		{
			loopstop=true;
			v_of_flists[index].Add(start);
			return true;
		}
	}
}


void MyAllocator::Create_v_of_flist(size_t size)//this initializes the vector of linked lists 
{
	int fibprev1 =0;
	int fibprev2 =1;
	int curfib =1;
	while(curfib*bbs <= size)
	{
		v_of_flists.push_back(FreeList(curfib));
		fibprev1= fibprev2;
		fibprev2= curfib;
		curfib = fibprev1 + fibprev2;
	}
}	


void MyAllocator::Merge(SegmentHeader* Segment1, SegmentHeader* Segment2)//merges two segment headers into one.
{
	PrintVector();
	cout<<"Merging ...\n";
	bool parentInh= Segment2->inh;
	bool parentLR= Segment1->inh;
	int index1 = findIndex(Segment1);
	int index2 = findIndex(Segment2);
	v_of_flists[index1].Remove(Segment1);
	cout<<"index1: "<<index1<<endl;
	cout<<"index2: "<<index2<<endl;
	cout<<"This is where it seg faults in the next line..\n";
	cout<<"Tries to access the index2 in the vector of lists and tries to remove something that is not there.\n";
	v_of_flists[index2].Remove(Segment2);
	Segment1->length = Segment1->length+Segment2->length;
	Segment1->inh = parentInh;
	Segment1->is_left= parentLR;
	int indexofParent = findIndex(Segment1);
	v_of_flists[indexofParent].Add(Segment1);
	
}

void MyAllocator::Split(SegmentHeader* Segment, int index)//splits the segment header into two 
{
	cout<<"Spliting block of size:"<<Segment->length<<endl;
	int index1 = index -1; //index of the right child
	int index2 = index - 2; // index of the left child
	bool parentinh = Segment->inh;
	bool parentLR = Segment->is_left;
	v_of_flists[index].Remove(Segment);
	SegmentHeader* newSeg = new((char*)Segment+ (size_t)(v_of_flists[index2].get_Fib_num()*bbs)) SegmentHeader(Segment->length-(size_t)(v_of_flists[index2].get_Fib_num()*bbs),true,parentinh,false); //segment for the right child
	v_of_flists[index1].Add(newSeg);
	//Now adjusting parent to be overwritten for the left child 
	Segment->length = (size_t)(v_of_flists[index2].get_Fib_num()*bbs);
	Segment->inh = parentLR;
	Segment->is_left = true;
	Segment->is_free =true;
	v_of_flists[index2].Add(Segment);
}


void MyAllocator::PrintVector()//prints each freelist and indices and fib number and lenght of each segment 
{
	for(int i=0;i<v_of_flists.size(); i++)
	{
		cout<<"Index:"<<i<<", Freelist's Fib Number: "<<v_of_flists[i].get_Fib_num()<<", Size of each segment header in this list: "<<v_of_flists[i].get_Fib_num()*bbs<<", number of segments in this list: "<<v_of_flists[i].getsize()<<endl;
	}
}

uint MyAllocator::roundtoNearestFibsize(uint i)//this is a helper function for round the input of malloc() to a block length that we have
{
	uint min = i+sizeof(SegmentHeader);
	if(min<bbs)//if the min is smaller than a basic block size then just set it to bbs
	{
		min= bbs;
	}
	else
	{
		for(int i=0; i<v_of_flists.size();i++)
		{
			if(v_of_flists[i].get_Fib_num()*bbs>min)
			{
				min = v_of_flists[i].get_Fib_num()*bbs;
				break;
			}
		}
	}
	return min;
}

int MyAllocator::findIndex(SegmentHeader* seg)//find the corresponding index according to the segment length
{
	int index =-1;
	for(int i=0;i<v_of_flists.size();i++)
	{
		if(v_of_flists[i].get_Fib_num()*bbs ==seg->length)
		{
			index =i;
			break;
		}
	}
	return index;
}

SegmentHeader* MyAllocator::getBuddy(SegmentHeader* Seg)//finds the buddy passed into the function
{
	if(Seg->is_left == false)//meaning if this segment is the right buddy then find its left buddy
	{
		int index = findIndex(Seg);
		size_t back = (size_t)(v_of_flists[index-1].get_Fib_num()*bbs);
		return (SegmentHeader*) (((char*)Seg)-back);//returns the left buddy segment header
	}
	else
	{
		return (SegmentHeader*)(((char*)Seg)+Seg->length);//returns the right buddy segment header
	}
}