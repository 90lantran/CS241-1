/** @file log.c */
#include <stdlib.h>
#include <string.h>
#include "log.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "log.h"
/**
 * Initializes the log.
 *
 * You may assuem that:
 * - This function will only be called once per instance of log_t.
 * - This function will be the first function called per instance of log_t.
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *   Pointer to the log data structure to be initialized.
 */

//struct _log_t * head; 
//struct _log_t * tail;

void log_init(log_t* l)
{
	l -> head = NULL; 
	l -> tail = NULL;
	l -> size = 0;  
}

/**
 * Frees all internal memory associated with the log.
 *
 * You may assume that:
 * - This function will be called once per instance of log_t.
 * - This funciton will be the last function called per instance of log_t.
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure to be destoryed.
 */
void log_destroy(log_t* l)
{
	/*
	int i; 
	char *temp;
	int s = l -> size;
	for (i = 0; i < s;i++){
		temp = log_pop(l);
	}
	*/

	/*

    ListNode *curr = head; 
    ListNode *next = curr -> next;
    
    while (curr != NULL){
		next = curr-> next;
		delete curr; 
		curr = next ;
   	}
   head = NULL; 
   tail = NULL; 
   length = 0; 


	*/


	if (l -> head == NULL && l -> tail == NULL){
   		//free(l);
   		//return;
   	}
   	else if (l -> head == l -> tail){
   		free(l -> head -> data);
   		free(l -> head);
   		//free(l -> tail);
		//free(l);
   	}
   	else {
 	   	log_entry_t * temp = l->head;
 	   	log_entry_t * next_one;// = temp -> next; 

   		while (temp != NULL){
	   		next_one = temp -> next;
	   		
	   		
	   		free(temp -> data);
	   		
	   		free(temp);
	   		temp = next_one;
	    	}
   		free(next_one);
   		free(temp);
	}
	//free(l-> head);
	//free(l -> tail);
	//free(l -> size);
	//free(l);
	return;
   /*
	log_entry_t * temp;
	log_entry_t * next_one; 

	for (temp = l -> head; temp != l -> tail; temp = temp -> next){
		next_one = temp -> next;
		free(temp -> data);
	}

	free(l -> tail -> data);
	*/
	//Set head and tail to NULL, size to 0
	//l -> head = NULL; 
	//l -> tail = NULL;
	//l -> size = 0; 

}

/**
 * Appends an item to the end of the log.
 *
 * The item MUST NOT be copied.  Only a pointer is stored in the log.
 *
 * You may assume that:
* - All pointers will be valid, non-NULL pointer.
*
 * @param l
 *    Pointer to the log data structure.
 * @param item
 *    Pointer to a string to be added to the log.
 */
void log_append(log_t* l, char *item)
{
	/* 1 CASES:
		1. Both head and tail are NULL
		2. General case
	*/

	//if (strlen(item) == 1){return;}

	// CASE 1
	if ((l -> head == NULL) && (l -> tail == NULL)){
		l -> head = malloc(sizeof(log_entry_t));
		l -> head -> data = item;
		l -> tail = l -> head; 
		l -> tail -> next = NULL; 
		l -> head -> prev = NULL; 
		l -> size++; 
	}
	// GENERAL CASE
	else {
		log_entry_t * temp = malloc(sizeof(log_entry_t)); 
		temp -> data = item; 
		l -> tail -> next = temp;
		temp -> prev = l -> tail;
		temp -> next = NULL;  
		l -> tail = temp; 
		l -> size ++; 
	}
}

/**
 * Removes and returns the last item in the log.
 *
 * If this function was called following a call to log_append(), the return
 * value will be the value that was just to the log.  If multiple calls are
 * made to log_pop(), is should continue to remove entries from the log in
 * a Last-In First-Out (LIFO) or "stack" order.
 *
 * You may assume that:
* - All pointers will be valid, non-NULL pointer.
*
 * @param l
 *    Pointer to the log data structure.
 *
 * @returns
 *    The last item in the log, or NULL if the log is empty.
 *
 */
char *log_pop(log_t* l)
{
	if (l->head == NULL && l -> tail == NULL){return NULL;}

	char * temp; 

	//pointing at the same thing
	if (l-> head == l -> tail){
		temp = l -> head -> data;
		l -> head = NULL;
		free(l -> tail); 
		l -> tail = NULL; 
		l -> size--; 
	}
	else {
		temp = l -> tail -> data;
		log_entry_t * back = l -> tail -> prev; 
		back -> next = NULL; 
		free(l->tail);
		l -> tail = back; 
		l -> size --;

	}
    return temp;
}

/**
 * Returns a pointer to the idx-th element in the log, where the
 * 0-th element is the first element appended to the log.
 *
 * You may assume that:
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure.
 * @param idx
 *    Zero-based index into the log, where the 0-th entry corresponds to the
 *    first (oldest) entry in the log and the (n-1)-th entry corresponds to
 *    the latest (newest) entry in the log.
 *
 * @returns
 *    The idx-th entry in the log, or NULL if such an extry does not exist.
 */
char *log_at(log_t* l, unsigned int idx) 
{
	char * back;

	if ((idx) > ((l -> size)-1) || (l->size ==0 && l->head == NULL && l-> tail == NULL)){
		back = NULL;
	}
	else {
		int i;
		log_entry_t * start = l -> head; 
		for (i = 0 ; i < (int)idx; i++){
			start = start -> next; 
		}
		back = start -> data;
	}
	return back;
}

/**
 * Returns the number of elements in the log.
 *
 * You may assume that:
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure.
 *
 * @returns
 *    Number of entires in the log.
 */
unsigned int log_size(log_t* l)
{
    return l ->size;
}

/**
 * Preforms a newest-to-oldest search of log entries for an entry matching a
 * given prefix.
 *
 * This search starts with the newest or (n-1)-th entry in the log and
 * compares each entry to determine if query is a prefix of the log entry.
 * Upon reaching a match, a pointer to that element is returned.  If no match
 * is found, a NULL pointer is returned.
 *
 * For example, a log may be built with five entries:
 * @code
 *    log_append(&l, "ab  1");
 *    log_append(&l, "a   2");
 *    log_append(&l, "abc 3");
 *    log_append(&l, "ab  4");
 *    log_append(&l, "a   5");
 * @endcode
 *
 * Using the log that was build above:
 * - A call to <tt>log_search(&l, "ab")</tt> will return the pointer to the string "ab  4"
 *   since the search must begin from the newest entry and move backwards.
 * - A call to <tt>log_search(&l, "a")</tt> will return the pointer to the string "a   5".
 * - A call to <tt>log_search(&l, "abc")</tt> will return the pointer to the string "abc 3".
 * - A call to <tt>log_search(&l, "d")</tt> will return a NULL pointer.
 *
 * You may assume that:
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure.
 * @param prefix
 *    The prefix to test each entry in the log for a match.
 *
 * @returns
 *    The newest entry in the log whose string matches the specified prefix.
 *    If no strings has the specified prefix, NULL is returned.
 */
char *log_search(log_t* l, const char *prefix)
{
	log_entry_t * temp; 

	for (temp = l -> tail; temp != NULL; temp = temp -> prev){
		if(strncmp(temp->data, prefix, strlen(prefix)) == 0){
			return temp ->data;
		}
	}
    return NULL;
}
