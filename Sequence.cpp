// FILE: Sequence.cpp
// CLASS IMPLEMENTED: sequence (see sequence.h for documentation)
// INVARIANT for the sequence ADT:
//   1. The number of items in the sequence is in the member variable
//      used;
//   2. The actual items of the sequence are stored in a partially
//      filled array. The array is a dynamic array, pointed to by
//      the member variable data. For an empty sequence, we do not
//      care what is stored in any of data; for a non-empty sequence
//      the items in the sequence are stored in data[0] through
//      data[used-1], and we don't care what's in the rest of data.
//   3. The size of the dynamic array is in the member variable
//      capacity.
//   4. The index of the current item is in the member variable
//      current_index. If there is no valid current item, then
//      current_index will be set to the same number as used.
//      NOTE: Setting current_index to be the same as used to
//            indicate "no current item exists" is a good choice
//            for at least the following reasons:
//            (a) For a non-empty sequence, used is non-zero and
//                a current_index equal to used indexes an element
//                that is (just) outside the valid range. This
//                gives us a simple and useful way to indicate
//                whether the sequence has a current item or not:
//                a current_index in the valid range indicates
//                that there's a current item, and a current_index
//                outside the valid range indicates otherwise.
//            (b) The rule remains applicable for an empty sequence,
//                where used is zero: there can't be any current
//                item in an empty sequence, so we set current_index
//                to zero (= used), which is (sort of just) outside
//                the valid range (no index is valid in this case).
//            (c) It simplifies the logic for implementing the
//                advance function: when the precondition is met
//                (sequence has a current item), simply incrementing
//                the current_index takes care of fulfilling the
//                postcondition for the function for both of the two
//                possible scenarios (current item is and is not the
//                last item in the sequence).

#include <cassert>
#include "Sequence.h"
#include <iostream>
using namespace std;

namespace CS3358_SP2023
{
   // CONSTRUCTORS and DESTRUCTOR
   sequence::sequence(size_type initial_capacity) : used(0), 
      current_index(0), capacity(initial_capacity)
   {
   	  //Check if the precondition is valid, if not
   	  //adjust the initial capacity to 1
      if(initial_capacity < 1)
	     capacity = 1;
	     
	  //dynamically allocate a new sequence memory   
	  data = new value_type[capacity];	  
   }

   sequence::sequence(const sequence& source) : used(source.used), 
      current_index(source.current_index), capacity(source.capacity)
   {
      //Allocate a new sequence memory dynamically
	  data = new value_type[capacity];
      
      //Deep copying of source data to new sequence
      for (size_type i = 0; i < used; ++i)
         data[i] = source.data[i];
   }

   sequence::~sequence()
   {
      //Deallocate all memory used by old sequence
      delete [] data;
      
   }

   // MODIFICATION MEMBER FUNCTIONS
   void sequence::resize(size_type new_capacity)
   {
      if(new_capacity < 1)
         new_capacity = 1;
      if(new_capacity < used)
	     new_capacity = used;
	  else
	     capacity = new_capacity;
		 
	  //Allocate a new memory to accommodate 
	  //a user-specified capacity
	  value_type * newData = new value_type[capacity];
	  
	  //Copy elements to new memory
	  for (size_t i = 0; i < used; ++i)
	     newData[i] = data[i];
		 
	  //Deallocate the old memory
	  delete [] data;
	  
	  //Set newData to be current data
	  data = newData; 	 	    
   }

   void sequence::start()
   {
      //The first item on the sequence becomes  
	  //the current item but if the sequence is 
	  //empty, then there is no current item.
	  current_index = 0;
   }

   void sequence::advance()
   {
      //check if precondition is valid
      //is_item returns true.otherwise quit
      assert(is_item());
      
      //If a sequence ontains item(s),current index
      //is the item immediately after the original
      //current index.
      current_index = current_index +1;
   }

   void sequence::insert(const value_type& entry)
   {
      //check if we have enough room to store new
      //items, otherwise resize using a resizing rule
      if (used == capacity)
         resize(size_t(1.5 * capacity) + 1);
         
      //continue to insert if room is available
	  if (!is_item())
	  {
	     //If no current item, insert the item
	     //and set it to current item.
	     current_index = 0;
	  	 for (size_t i = used + 1; i > current_index; --i)
	  	    data[i] = data[i-1];
	  	 data[current_index] = entry;
		 used++;     
	  }
	  else
	  {
	     //In case there is item(s) in a sequence, move every item
	     //to the right to reserve a room for new item and place
	     //it before current item. thus entry is current item
		 for(size_t i = used + 1; i >current_index; --i)
		    data[i] = data[i-1];
		 data[current_index] = entry;
		 used++;   
	  }   
   }

   void sequence::attach(const value_type& entry)
   {
      //check if we have enough room, otherwise resize
	  if (used == capacity)
         resize(size_t(1.5 * capacity) + 1);
         
      if (!is_item())
      {
      	 //If no item in a sequence,insert new item.the new
		 //item is now the current item of the sequence.
         data[current_index] = entry;
		 used++;	
	  }
	  else
	  {
	     //If sequence has item(s), the new current item will be 
	     //the item inserted after the previous current item
		 current_index = current_index + 1;
		 
		 //move item(s) after current item to the right to reserve 
		 //a room for new item and place new item after current item. 
		 //thus newly entered item is now the current item
		 for (size_t i = used + 1; i > current_index; --i)
		    data[i] = data[i-1];
		 data[current_index] = entry;
		 used++;		
	  }
   }

   void sequence::remove_current()
   {
      //check if is_item() valid
	  assert(is_item());
	  
	  //if the item exist in sequence, remove it and
	  //move all items after it to the left
	  for (size_t i = current_index; i < used-1; ++i)
	     data[i] = data[i+1];
	   used--; 	  
   }

   sequence& sequence::operator=(const sequence& source)
   {
      if (this != &source)
      {
         //Dynamically allocate a newData memory 
         //with the same size as source sequence
		 value_type * newData = new value_type[source.capacity];
		 
		 //copy items of source sequence into newData
		 for (size_t i = 0; i < source.used; ++i)
		    newData[i] = source.data[i];
		    
		 //Deallocate the old memory
		 delete [] data;
		 
		 //now newData has current member values
		 data = newData;
		 capacity = source.capacity;
		 used = source.used;
		 current_index = source.current_index;
		 	 
	  }
      return *this;
   }

   // CONSTANT MEMBER FUNCTIONS
   sequence::size_type sequence::size() const
   {
      //return the number of items in a sequence
      return used;
   }

   bool sequence::is_item() const
   {
      //return true if there is a valid "current" item 
	  //that may be retrieved by activating current()
      return (current_index != used); 
   }

   sequence::value_type sequence::current() const
   {
      //check if is_item() is true
      assert(is_item());
      
      //if the sequence has item(s), return current item
      return data[current_index]; 
   }
}

