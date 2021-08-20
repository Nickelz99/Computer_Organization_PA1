#include "Ackerman.h"
#include "BuddyAllocator.h"
#include <unistd.h>
#include <stdio.h>

void easytest(BuddyAllocator* ba){
  // be creative here
  // know what to expect after every allocation/deallocation cycle

  // here are a few examples
  ba->printlist();
  // allocating a byte
  char * mem = ba->alloc (1);
  // now print again, how should the list look now
  ba->printlist ();

  //ba->merge((BlockHeader*)mem, ba->FreeList[0].head);
  

  ba->free (mem); // give back the memory you just allocated
  //cout << "Just passed mem" << endl; 
  ba->printlist(); // shouldn't the list now look like as in the beginning

}

int main(int argc, char ** argv) {
  int basic_block_size = 128, memory_length = 128 * 1024 * 1024;
  int option;
  while((option = getopt(argc, argv, "b:s:"))!=-1)
  {
    switch (option)
    {
      case 'b':
      basic_block_size = atoi(optarg);
      break;
  
      case 's':
      memory_length = atoi(optarg);
      break;
    }
  }

  // create memory manager
  BuddyAllocator * allocator = new BuddyAllocator(basic_block_size, memory_length);

  // the following won't print anything until you start using FreeList and replace the "new" with your own implementation
  easytest (allocator);

  
  // stress-test the memory manager, do this only after you are done with small test cases
  Ackerman* am = new Ackerman ();
  am->test(allocator); // this is the full-fledged test.
  allocator ->printlist();
  // destroy memory manager
    
//    char* m1 = allocator->alloc(2000);
//    char* m2 = allocator->alloc(100);
//    char* m3 = allocator->alloc(240);
//
//    allocator->free(m3);
//    allocator->free(m2);
  delete allocator;
}
