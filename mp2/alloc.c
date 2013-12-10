/** @file alloc.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "debug.h"

/**
 * Allocate space for array in memory
 * 
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 * 
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size)
{
	/* Note: This function is complete. You do not need to modify it. */
	void *ptr = malloc(num * size);
	
	if (ptr)
		memset(ptr, 0x00, num * size);

	return ptr;
}

/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */
 

 
 /*
 	Bucket 1 -  1-16
 	Bucket 2 -  16-64
 	Bucket 3 -  64-128
 	Bucket 4 -  128 - MAX??
 */
 
 typedef struct mdata
 {
	int  usage;
        int  capacity;
        //void * data;
        char  isFree; // 0 for false, 1 for true
        struct  mdata * next;
        struct  mdata * back;
 } mdata;
    
    mdata * b1 = NULL;
    mdata * b2 = NULL;
    mdata * b3 = NULL;
    mdata * b4 = NULL;
 
void *malloc(size_t size)
{
	mdata * looper = NULL;
	mdata * t = NULL; 
	 	
	
	//*******Find out which bucket the search****************// 
	if (size <= 4096){t = b1;}
	else if (size <= 4194304){t = b2;}
	else if (size <= 536870892) {t = b3;}
	else {t = b4;}
	
	
	//*******If we actually have memory locations in the 
	if (t != NULL){
		//******Find the most compatible memory location ********// 
		mdata * best = t; 	
		for (looper = t; looper!=NULL; looper = looper -> next) {
			if (looper -> isFree == 1){
				if ( (((int)looper -> capacity - (int)size) < ((int)best -> capacity - (int)size)) && ((int)size <= (int)looper -> capacity) && ((int)size <= (int)best -> capacity)  ){
					best = looper;
				}
			}
		}
		
		// *********Now we have found the Best memory location, and we have to remove it from the Free Bucket List *****// 
		/*
			There are 3 Cases
			1. Head of the list
			2. Middle of the list
			3. End of the list
		
		*/
		//*********If the only one, remove it*********//
				
		if (best -> next == NULL && best -> back == NULL){
			if (size <= 4096){b1 = NULL;}
			else if (size <= 4194304){b2 = NULL;}
			else if (size <= 536870892) {b3 = NULL;}
			else {b4 = NULL;}
			best -> isFree = 0; // just in case
		}
		//*********If it is the first one*************//
		else if (best -> back == NULL && best -> next != NULL){
			best -> next -> back = NULL; 
			if (size <= 4096){b1 = best -> next;}
			else if (size <= 4194304){b2 = best -> next;}
			else if (size <= 536870892) {b3 = best -> next;}
			else {b4 = best -> next;}
			best -> isFree = 0; 
		
		}
		
		//*********In the middle of the list, remove it*********//
		else if (best -> next != NULL && best -> back != NULL){
			best -> next -> back = best -> back;
			best -> back -> next = best -> next;
			best -> isFree = 0; // just in case
		}
		//*********It is at the end of the list, remove it********//		
		else{
			best -> back -> next = NULL; 
			best -> isFree = 0; // just in case
		}	
		
		if ((int)size <= (int)best -> capacity){	
			return ((char *) best) + sizeof(mdata);
		}

	}
	
	mdata * created = sbrk(sizeof(mdata) + size);
	created -> usage = size; 
	created -> capacity = size; 
	created -> isFree = 0; 
	 
	return ((char *) created) + sizeof(mdata); 
}
 

/**
 * Deallocate space in memory
 * 
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
 

void free(void *ptr)
{
	// "If a null pointer is passed as argument, no action occurs."
	if (!ptr){return;}
	
	mdata * temp = NULL; 
	temp =(mdata*) (ptr - sizeof(mdata));
	temp -> isFree = 1; // set it to Free
	
	
	
	void * end = sbrk(0);
	
	if ((void *)temp + temp -> capacity + sizeof(mdata) >= end) {brk(temp); return;}
	
	if (temp -> capacity <= 4096) {
		if (b1 == NULL){
			temp -> next = NULL; 
			temp -> back = NULL; 
			b1 = temp; 
		}
		else {
			b1 -> back = temp; 
			temp -> next = b1; 
			temp -> back = NULL; 
			b1 = temp; 
		}
	}
	else if((temp -> capacity) <= 4194304) {
		if (b2 == NULL){
			temp -> next = NULL; 
			temp -> back = NULL; 
			b2 = temp; 
		}
		else {
			b2 -> back = temp; 
			temp -> next = b2; 
			temp -> back = NULL; 
			b2 = temp;
		}
	}
	else if((temp -> capacity) <= 536870892) {
		if (b3 == NULL){
			temp -> next = NULL; 
			temp -> back = NULL; 
 			b3 = temp; 
		}
		else {
			b3 -> back = temp; 
			temp -> next = b3; 
			temp -> back = NULL; 
			b3 = temp; 
		}
	}
	else {
		if (b4 == NULL){
			temp -> next = NULL; 
			temp -> back = NULL; 
			b4 = temp; 
		}
		else {
			b4 -> back = temp; 
			temp -> next = b4; 
			temp -> back = NULL; 
			b4 = temp; 
		}
	}
	

	
	return;
}

/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block (mdata*)previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *    
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size)
{
	 // "In case that ptr is NULL, the function behaves exactly as malloc()"
	if (!ptr){return malloc(size);}

	 // "In case that the size is 0, the memory previously allocated in ptr
	 //  is deallocated as if a call to free() was made, and a NULL pointer
	 //  is returned."
	
	if (size == 0){return NULL;}
	
	
	
	mdata * curr = NULL; 
	curr = (mdata*)((ptr) - sizeof(mdata));
	
	//void * end = sbrk(0);
 	//DPRINTF("what you at right now %p, What the max is %p", curr, end);
 	
	// **********If it's the ABOUT the same size******************	
	if ((int)size <= (curr -> capacity)){
		curr -> usage = size;
		return ptr;
	}		
		
	void * temp = malloc(size); 
	
	if ((int)size < curr -> usage) // ASKING FOR SMALLER 
		memcpy(temp, ptr, size);
	else { // ASKING FOR LARGER
		memcpy(temp, ptr,curr -> usage);
	}
	
	free(ptr); 
	
	return temp; 

}

