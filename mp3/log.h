/** @file log.h */

#ifndef __LOG_H_
#define __LOG_H_

/** The log data structure. */


typedef struct _log_entry_t
{
	struct _log_entry_t * next;
	struct _log_entry_t * prev;  
	char * data; 
} log_entry_t;

typedef struct _log_t
{
	log_entry_t * head;
	log_entry_t * tail; 
	size_t size; 
} log_t;


void log_init(log_t* l);
void log_destroy(log_t* l);

void log_append(log_t* l, char *item);
char *log_pop(log_t* l);
char *log_at(log_t* l, unsigned int idx);
unsigned int log_size(log_t* l);
char *log_search(log_t* l, const char *prefix);

#endif
