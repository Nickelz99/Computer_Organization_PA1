#ifndef _BuddyAllocator_h_                   // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;
typedef unsigned int uint;

/* declare types as you need */

class Error{};

class BlockHeader{
public:
	// think about what else should be included as member variables
	bool Free = true;
	int block_size;  // size of the block
	BlockHeader* next = nullptr; // pointer to the next block
};

class LinkedList{
	// this is a special linked list that is made out of BlockHeader s. 
public:
	BlockHeader* head;		// you need a head of the list
public:
	void insert (BlockHeader* b)
	{	// adds a block to the list
		if(head == nullptr)
		{
			head = b;
			b->next = nullptr;
		}
		else
		{
			BlockHeader* temp = head->next;
			head->next = b;
			b->next = temp;	
		}
	}

	void remove (BlockHeader* b){  // removes a block from the list
		if (b == head)
		{
			if (head != nullptr)
			{
				head = head->next;
			}
			
		}
		else
		{
			BlockHeader* temp = head;
			BlockHeader* temp2 = temp;
			while (temp != b && temp!=nullptr)
			{
				temp2 = temp;
				temp = temp->next;
			}
			if (temp2 != nullptr)
			{
				temp2->next = b->next;
			}
		}
	
	}
};


class BuddyAllocator{
private:
	/* declare more member variables as necessary */
	char* start;
	int k_size;
	vector<LinkedList> FreeList;
	int basic_block_size;
	int total_memory_size;


private:
	/* private function you are required to implement
	 this will allow you and us to do unit test */
	
	BlockHeader* getbuddy (BlockHeader * addr); 
	// given a block address, this function returns the address of its buddy 
	
	bool arebuddies (BlockHeader* block1, BlockHeader* block2);
	// checks whether the two blocks are buddies are not

	BlockHeader* merge (BlockHeader* block1, BlockHeader* block2);
	// this function merges the two blocks returns the beginning address of the merged block
	// note that either block1 can be to the left of block2, or the other way around

	BlockHeader* split (BlockHeader* block);
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected


public:
	BuddyAllocator (int _basic_block_size, int _total_memory_length); 
	//start = new char[total mem]; goes in free list
	// fix Free lists size using k
	// k = log base 2 (total mem/bbs)
	//Fl[k] head = start
	//FL[k] head-> BS = S
	//FL[k] head-> next = null
	// for i = 0; k-1
	//FL[i]head = null
	/* This initializes the memory allocator and makes a portion of 
	   ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
	   its minimal unit of allocation. The function returns the amount of 
	   memory made available to the allocator. If an error occurred, 
	   it returns 0. 
	*/ 

	~BuddyAllocator(); 
	/* Destructor that returns any allocated memory back to the operating system. 
	   There should not be any memory leakage (i.e., memory staying allocated).
	   //delete start;
	*/ 

	char* alloc(int _length); 
	/* Allocate _length number of bytes of free memory and returns the 
		address of the allocated portion. Returns 0 when out of memory. 
		//rx = x+sizeof(BH)
		// i = f(rx,bbs)
		//if(FL[i]. head)
		{
			Bh*b=FL[i].head;
			FL[i].remove(b);
			return (char *)(b+1);
			
		}
		//else
		{
			int j = i
			while(FL[j]*head == 0)&&j<k)
			{j++}
			if(j>k){return nullptr}
			while(j>i)
			{
				BH*b = FL[j].head;
				BH*bb = split(b)
				FL[j].remove b
				FL[j-1].insert(b)
				FL[j-1].insert(bb)
				j--	
			}
		}
		
		
	*/ 
	int free(char* _a); 
	/* Frees the section of physical memory previously allocated 
	   using ’my_malloc’. Returns 0 if everything ok. */ 
   
	void printlist ();
	/* Mainly used for debugging purposes and running short test cases */
	/* This function should print how many free blocks of each size belong to the allocator
	at that point. The output format should be the following (assuming basic block size = 128 bytes):

	[0] (128): 5
	[1] (256): 0
	[2] (512): 3
	[3] (1024): 0
	....
	....
	 which means that at this point, the allocator has 5 128 byte blocks, 3 512 byte blocks and so on.*/
};

#endif 
