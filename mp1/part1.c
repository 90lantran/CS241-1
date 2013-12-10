/** @file part1.c */

/*
 * Machine Problem #1
 * CS 241
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mp1-functions.h"

/**
 * (Edit this function to print out the ten "Illinois" lines in mp1-functions.c in order.)
 */
int main()
{
	/* #1 */
	first_step(81);

	/* #2 */
	int x = 132; 
	second_step(&x);
	
	/* #3 */
	int ** send;
	int * temp; 
	int act = 8942;
	temp = &act; 
	send = &temp; 
	double_step(send);
	 
	/* #4 */
	int *p = 0; 
	strange_step(p); 
	
	/* #5 */
	char * five = "hi!"; 
	void * fivep = five; 
	empty_step(fivep);
	
	/* #6 */
	char * six = "sssu";
	void * sixp = six; 
	two_step(sixp, six);
	
	
	/* #7 WTF */
	char a = 'A'; 
	char b = 'C'; 
	char e = 'E'; 
	three_step(&a, &b, &e); 
	
	/* #8 */
	char * eight_first = "fAf";
	char * eight_second = "ffIf";
	char * eight_third = "fffQf";
	
	step_step_step(eight_first,eight_second,eight_third);
	/* #9 */
	char c = 'A'; 
	int i = 'A'; 
	it_may_be_odd(&c, i); 
	
	/* #10 */

	int * ten_one; 
	int * ten_two; 
	
	ten_two = malloc(sizeof(int));
	
	ten_one = ten_two; 
	
	*ten_two = 2049; 
	
	the_end(ten_two, ten_one);	
	free(ten_two);


	return 0;
}
