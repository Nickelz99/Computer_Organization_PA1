#include "BuddyAllocator.h"
#include <iostream>
#include <cmath>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
start = new char[_total_memory_length];
k_size = log2(_total_memory_length/_basic_block_size);
basic_block_size = _basic_block_size;
total_memory_size = _total_memory_length;
FreeList.resize(k_size+1);
BlockHeader * mem = (BlockHeader *) start;
for (int i = 0; i < k_size+1; i++)
{
  FreeList[i].head=nullptr;
}
FreeList[k_size].insert(mem);
mem ->next = NULL;
mem ->Free = true;
mem ->block_size = total_memory_size;
}

BuddyAllocator::~BuddyAllocator (){
	delete start;
}

char* BuddyAllocator::alloc(int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  //return new char [_length];
//    cout << "alloc - FreeList 0: " << (void*) FreeList.at(0).head << endl;

  int temp_size = _length + sizeof(BlockHeader); // total size of block
  vector<int>sizes;
  sizes.resize(log2(total_memory_size/basic_block_size)+1);
  sizes[0] = basic_block_size;
  for (int i = 1; i < sizes.size(); i++) // finds all possible sizes
  {
    sizes[i] = sizes[i-1] * 2;
  }
  int req_size = 0;
  int req_size_loc = 0;
  for (int i = 0; i < sizes.size(); i++) // finds required size of block
  {
    if(sizes[i]>temp_size)
    {
      req_size = sizes[i];
      req_size_loc = i;
      break;
    }
  }
  
  if (FreeList[req_size_loc].head) // returns the address of the block as a char ptr if it's in the free list
  {
    BlockHeader* temp = FreeList[req_size_loc].head;
    FreeList[req_size_loc].remove(temp);
    temp->Free = false;
      
    char* ret  = (char*)temp + sizeof(BlockHeader);
      //cout << "alloc - block size: " << temp->block_size << " char*: " << (void*)ret << " BlockHeader* " << temp << endl;

    return ret;
  }
  else // searches for block of larger size then breaks it down
  {
    int j = req_size_loc;
    while ((FreeList[j].head == nullptr)&&j<k_size)
    {
      j++;
      if(j>k_size)
      {
        return nullptr;
      }
    }
    while(j>req_size_loc)
			{
				BlockHeader*b1 = FreeList.at(j).head; 
				BlockHeader*b2 = split(b1);
				FreeList[j].remove(b1); // moves blocks
				FreeList[j-1].insert(b1);
        //cout << b1->block_size << "row number " << j-1 <<  endl;
				FreeList[j-1].insert(b2);
        //cout << b2->block_size <<  "row number " << j-1 << endl;
				j--;
			}
    BlockHeader* temp2 = FreeList[req_size_loc].head; // returns the block now that it is broken down
    FreeList[req_size_loc].remove(temp2);
    temp2->Free = false;
    //char* ret2  = (char*)temp2 + 1;
      
    char* ret2  = (char*)temp2 + sizeof(BlockHeader);
      //cout << "alloc - block size: " << temp2->block_size << " char*: " << (void*)ret2 << " BlockHeader* " << temp2 << endl;

    return ret2;
  
  }
  
}

BlockHeader* BuddyAllocator::split (BlockHeader* block)
{
  int temp_size = block->block_size;                // finding block size and creating block 2
  uintptr_t block_start = (uintptr_t) block;
  uintptr_t block2_start = block_start + (temp_size/2);
  BlockHeader* block2 = (BlockHeader*) block2_start; 
  block ->block_size = (temp_size/2);       // setting all variables for block2 header and redoing original block size
  block2 ->block_size = (temp_size/2);
  block2 -> next = nullptr;
  block2 -> Free = true;
  
  //int temp = temp_size/basic_block_size;    // finding old position
  //int pos = log2(temp);
  //int temp2 = (temp_size/2)/basic_block_size; // finding new position
  ///int pos2 = log2(temp2);
  //FreeList[pos].remove(block);                // moving blocks into different linked list
  //FreeList[pos2].insert(block);
  //FreeList[pos2].insert(block2);
  return block2;

}

BlockHeader* BuddyAllocator::merge (BlockHeader* block1, BlockHeader* block2)
{
  int temp_size = block1 ->block_size;
    uintptr_t b1 = (uintptr_t)block1;
    uintptr_t b2 = (uintptr_t)block2;
    
    if(block1->block_size != block2->block_size){
        cerr << "Error in merge. Block sizes do not eqaul each other";
        return nullptr;
    }
    
  if(b1 < b2)
  {
    block1 ->block_size = temp_size*2;
    int temp = temp_size/basic_block_size;    // finding old position
    int pos = log2(temp);
    FreeList[pos].remove(block1);                // moving blocks into higher linked list
    FreeList[pos].remove(block2);
    int temp2 = (temp_size*2)/basic_block_size; // finding new position
    int pos2 = log2(temp2);
    FreeList[pos2].insert(block1);
    return (block1);
  }
  else
  {
    block2 ->block_size = temp_size*2;
    int temp = temp_size/basic_block_size;    // finding old position
    int pos = log2(temp);
    FreeList.at(pos).remove(block1);                // moving blocks into higher linked list
    FreeList.at(pos).remove(block2);
    int temp2 = (temp_size*2)/basic_block_size; // finding new position
    int pos2 = log2(temp2);
    FreeList.at(pos2).insert(block2);
    return (block2);
  }

}
bool BuddyAllocator::arebuddies (BlockHeader* block1, BlockHeader* block2)
{
  if(block1->block_size != block2->block_size)
  {
    return false;
  }
  else
  {
    uintptr_t temp_1 = (uintptr_t)block1 - (uintptr_t)start;
    uintptr_t temp_2 = (uintptr_t)block2 - (uintptr_t)start;
    if ((temp_1 ^ temp_2) == block1->block_size)
    {
      return true;
    }
    else
    {
      return false;
    }
    
    
  }
  
}

BlockHeader* BuddyAllocator::getbuddy (BlockHeader * addr)
{
  long temp = addr ->block_size/basic_block_size;    // finding position
  long pos = log2(temp);
  BlockHeader * temp_1 = FreeList[pos].head;
  //BlockHeader* temp_2 = temp_1;
  bool test = false;
  while (temp_1 != nullptr)
  {
    test = arebuddies(temp_1 , addr);
    if (test)
    {
      return temp_1;
    }
    temp_1 = temp_1->next;
    if(temp_1 == nullptr)
    {
      return nullptr;
    }
    
  }
    return nullptr;
}



int BuddyAllocator::free(char* _a) {
  /* Same here! */
//    cout << "free - FreeList 0: " << (void*) FreeList.at(0).head << " input: " << (void*) _a << endl;
    int i = 0;
  BlockHeader* temp = (BlockHeader*)(_a - sizeof(BlockHeader));
  temp->Free = true;
    //cout << "free - block size: " << temp->block_size << " char*: " << (void*) _a << " BlockHeader* : " << temp <<  endl;
  BlockHeader* temp_2 = getbuddy(temp);
    long hold_size = temp ->block_size/basic_block_size;
    long pos = log2(hold_size);
    FreeList[pos].insert(temp);

  if (temp_2 == nullptr)
  {
    return -1;
  }
  while (temp_2->Free == true && temp_2->block_size < total_memory_size)
  {
    temp_2 = getbuddy(temp);
    if (temp_2 == nullptr || temp == nullptr)
    {
      return -1;
    }
    else
    {
      temp = merge (temp, temp_2);
    }
      ++i;
  }
    printlist();
  return 1;
}

void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  for (int i=0; i < FreeList.size(); i++){
    cout << "[" << i <<"] (" << ((1<<i) * basic_block_size) << ") : ";  // block size at index should always be 2^i * bbs
    int count = 0;
    BlockHeader* b = FreeList[i].head;
    // go through the list from head to tail and count
    while (b){
      count ++;
      //cout << endl << b->block_size << "block size " << endl;
      // block size at index should always be 2^i * bbs
      // checking to make sure that the block is not out of place
      if (b->block_size != (1<<i) * basic_block_size){
        cerr << "ERROR:: Block is in a wrong list" << endl;
          throw Error();
        exit (-1);
      }
      b = b->next;
    }
    cout << count << endl;
  }
}
/*
void BuddyAllocator::printlist (){
    cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
    for (int i=0; i<FreeList.size(); i++){
        cout << "[" << i <<"] (" << ((1<<i) * basic_block_size) << ") : ";  // block size at index should always be 2^i * bbs
        int count = 0;
        BlockHeader* b = FreeList [i].head;
        // go through the list from head to tail and count
        stringstream ss;
        while (b){
            count ++;
            ss << (void*)(b) << " ";
            // block size at index should always be 2^i * bbs
            // checking to make sure that the block is not out of place
            if (b->block_size != (1<<i) * basic_block_size){
                cerr << "ERROR:: Block is in a wrong list" << endl;
                throw Error();
                exit (-1);
            }
            b = b->next;
        }
        cout << count << " address " << ss.str() << endl;
    }
}
*/