/* 
    File: memtest.c

    Author: R. Bettati
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : 2019/09/02

    Test program for the memory allocator in MP1 of CSCE 313.
    This program reads and processes the command-line arguments and then
    exercises the memory allocator by allocating and de-allocating memory
    as part of the recursive invocations in the function 'ackerman', which 
    is famous for being very highly recursive.
*/
/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */
    
/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<iostream>
#include<string.h>
#include<sys/time.h>
#include<assert.h>
#include<unistd.h>
#include "my_allocator.hpp"

/*--------------------------------------------------------------------------*/
/* NAME SPACES */ 
/*--------------------------------------------------------------------------*/

using namespace std;
/* I know, it's a bad habit, but this is a tiny program anyway... */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* GLOBAL DATA */
/*--------------------------------------------------------------------------*/

MyAllocator * my_allocator;
/* We are defining our allocator as a global resource. */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

void ackerman_main(size_t _block_size, size_t _mem_size);
/* Repeatedly asks user for two parameters, 'a' and 'b', and computes the 
   result of the (highly recursive!) ackerman function. 
   During every recursion step, it allocates and de-allocates a portion 
   of memory with the use of the memory allocator defined in module 
   "my_allocator.H".
*/ 

int ackerman(int a, int b);
/* This is the implementation of the Ackerman function. The function itself 
   is very simple (just two recursive calls). We use it to exercise the 
   memory allocator (see "MyAllocator::Malloc" and "MyAllocator::Free"). 
   We use two additional calls to "gettimeofday" to measure the elapsed time.
*/

unsigned long num_allocations;
/* Global counter to keep track of the number of allocations exercised
   during one test run.
*/

void print_time_diff(const string _label,
		     const struct timeval & _tp1,
		     const struct timeval & _tp2);
/* Prints to stdout the difference, in seconds and museconds, between
   two timevals. 
*/

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  //Source: https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html 
  //Helped me understand how to use getopt function
  extern char* optarg;		//pointer to the current parameter in the command line
  int c;				//used for getopt function
  size_t block_size= 128; //default size is 128 bytes
  size_t mem_size  = 1024*512;//default size is 512KB
  while((c = getopt(argc, argv, "b:s:")) != -1)//while loop with swictch for parsing of command line
	  switch (c)
	  {
		case 'b':
				block_size = atoi(optarg);
				break;
		case 's':
				mem_size = atoi(optarg);
				break;
		case '?':
				cout<<"Only use this format: ./memtest -b <blocksize> -s <memsize> \n";
	  }
  ackerman_main(block_size, mem_size);

}

/*--------------------------------------------------------------------------*/
/* DRIVER FOR THE ACKERMAN FUNCTION */
/*--------------------------------------------------------------------------*/

void ackerman_main(size_t _block_size, size_t _mem_size) {
  /* This function first creates an allocator with given block and memory size.
     It then repeatedly first asks the user for the two parameters
     'a' and 'b' to pass to the Ackerman function, and then invokes the 
     Ackerman function.
     Before and after the invocation of the ackerman function, the 
     value of the wallclock is taken, and the elapsed time for the 
     computation of the ackerman function is displayed.
  */
  
  int a, b; /* Parameters for the invocation of the Ackerman function. */ 

  struct timeval tp_start; /* Used to compute elapsed time. */
  struct timeval tp_end;

  for (;;) { /* Loop forewer, or until we break. */

    num_allocations = 0;

    cout << endl;
    cout << "Please enter parameters 'a' and 'b' to ackerman function." << endl;
    cout << "Note that this function takes a long time to compute" << endl;
    cout << "even for small values. Keep 'a' at or below 3, and 'b'" << endl;
    cout << "at or below 8." << endl;
    cout << "Otherwise, the function takes seemingly forever." << endl;
    cout << "Enter 0 for either 'a' or 'b' in order to exit" << endl << endl;

    cout << "  a = "; cin >> a;
    if (a <= 0) break;
    cout << "  b = "; cin >> b;
    if (b <= 0) break;

    cout << "      a = " << a << ", b = " << b << endl;

    my_allocator = new MyAllocator(_block_size, _mem_size);
    
    assert(gettimeofday(&tp_start, 0) == 0);
    /* Assert aborts if there is a problem with gettimeofday.
       We rather die of a horrible death rather than returning
       invalid timing information! 
    */
    
    int result = ackerman(a, b); 
    /* We really don't care about the result. It's like Zen:
       It's the way there that counts; not the result that we find there. 
    */
    
    assert(gettimeofday(&tp_end, 0) == 0);
    /* (see above) */

    delete my_allocator;

    cout << "Result of ackerman(" << a << ", " << b << "):" << result << endl;

    print_time_diff("Time taken for computation: ", tp_start, tp_end);
  
    cout << "Number of allocate/free cycles: " << num_allocations << endl;
    cout << endl << endl;

  }
  
  cout << "Reached end of Ackerman program. Thank you for using it" << endl;

}

/*--------------------------------------------------------------------------*/
/* IMPLEMENTATION OF ACKERMAN FUNCTION */
/*--------------------------------------------------------------------------*/

int ackerman(int a, int b) {
/* This is the implementation of the Ackerman function. 
   The function itself is very simple (just two recursive calls). 
   We use it to exercise the memory allocator 
   (see "MyAllocator::Malloc" and "MyAllocator::Free"). 
   We use two additional calls to "gettimeofday" to measure the elapsed time.
*/

  /* Choose randomly the size of the memory to allocate: */
  int to_alloc =  ((2 << (rand() % 19)) * (rand() % 100)) / 100;
  if  (to_alloc < 4) to_alloc = 4;

  int result = 0;
  
  /* Here we allocate the memory using our own allocator. */
  char * mem = (char*)my_allocator->Malloc(to_alloc * sizeof(char));

  num_allocations++;

  if (mem == nullptr) {

    cerr << "ALLOCATOR FAILED: PROBABLY OUT OF MEMORY." << endl;
    
  }
  else {

    /* Fill the allocated memory with a value. 
       This same value will be used later for testing. 
    */
    char c = rand() % 128;
    memset(mem, c, to_alloc * sizeof(char));

    /* Here we do the Ackerman recursion thing. */
    if (a == 0)
      result = b + 1;
    else if (b == 0)
      result = ackerman(a - 1, 1);
    else
      result = ackerman(a - 1, ackerman(a, b - 1));

    /* We returned from recursion. 
       Let's check if the memory allocated above still has the same value. 
    */
    for (int i = 0; i < to_alloc; i++) {
      if (mem[i] != c) {
	/* The value does not match. 
	   It must have been overwritten somewhere. This is very bad. 
	*/
	cout << "Memory checking error!" << endl;
	assert(false);
      }
    }

    /* Now we free the memory allocated above; we use our own allocator. */
    assert(my_allocator->Free(mem)); /* We crash if Free() fails */
  }

  return result;
  /* Generally the Ackerman function returns a value. 
     We don't really care for it. 
  */
}

/*--------------------------------------------------------------------------*/
/* UTILITY FUNCTIONS */
/*--------------------------------------------------------------------------*/

void print_time_diff(const string _label, 
                     const struct timeval & _tp1, 
                     const struct timeval & _tp2) {
  /* Prints to stdout the difference, in seconds and museconds, between
     two timevals. 
  */

  cout << _label;
  long sec = _tp2.tv_sec - _tp1.tv_sec;
  long musec = _tp2.tv_usec - _tp1.tv_usec;
  if (musec < 0) {
    musec += 1000000;
    sec--;
  }
  cout << " [sec = " << sec << ", musec = " << musec << "]" << endl;

}


