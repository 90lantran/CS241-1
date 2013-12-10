/*
CS 241: Homework #1
DUE DATE: On the SVN at 11:59pm, Wednesday, Jan. 22, 2013 (No late submissions!)

General Instructions:
- This assignment is a SOLO assignment.
- Each question is worth equal weight.
- The assignment MUST be committed to your SVN by the due date.  For
  instructions on how to use SVN and other course information relating
  to online submission, please see:
    http://courses.engr.illinois.edu/cs241/handin.html


[Part I]
Each question consists of a C code snippet that contains one major problem.
The problems range from compile-time errors, misuse of C functions or C
syntax, and logical errors.

In some questions, unnecessary code is eliminated and replaced with "...".

In each case assume that functions without return statements may contain 
subsequent instructions potentially referring to declared local variables 
(e.g. do not change the types of variables declared in these functions).

Please refrain from making changes to function prototypes.
 
For each problem, you must do two things:
  (a): State what the code attempts to do.
  (b): Correct the code.
  (c): Explain the problem with the provided code.
*/

/* ===== Problem #1 ===== */
/*Goal: Have a pointer to the value of x. 
  Problem: No memory has been allocated to set the pointer to a number, this is setting the address to point to as x, the pointer should equal the memory location of x, hence the &.  
*/
void Problem1(){
	int x;
	...
	
	int *p = &x;
}


/* ===== Problem #2 ===== */
/*Goal: Create a pointer variable that pointed to a float
  Problem: No memory was allocated for the float
*/
void Problem2(){
	float *p = malloc(sizeof(float));
	*p = 12.5;
}


/* ===== Problem #3 ===== */
/*Goal: Have the if statement run when a is between 0 and 1
  Problem: The boolean statement in the if statement has to be split in two conditions. 
*/
void Problem3(){
	float a;
	...
	if((0 < a)  && (a < 1))
		printf("a is a value between 0 and 1.\n");
}

/* ===== Problem #4 ===== */
/*Goal: Have the first part of the if statement run when the two integers are equal to one another
 Problem: If both values are assigned values, then the first statement will always be true and only otherwise
 print the else statement. The original code is not doing a check, whereas x ==y is. 
*/
void Problem4(){
	int x=5;
	int y;
	...
	if(x==y){
		printf("x and y are equal.\n");}
	else
		{printf("x and y are different.\n");}
	
}

/* ===== Problem #5 ===== */
/*Goal: The purpose of this function is to return the float value of the location pointed to by x. 
  Problem: The current code is taking the code is casting a memory location and is taking the float of that. 
  The new code is taking the float of the actual integer value and successfully returning it. 
*/
float Problem5(int *x){
	float y = (float ) *x;
	return y;
}

/* ===== Problem #6 ===== */
/*Goal: Continue the while loop while the a is not equal to b. 
  Problem: The ! and = were switched in order, before it was =!, the correct way is !=. 
*/
void Problem6(){
	int a=0, b=10;
	while (a != b){
		...
		a++;
	}
}

/* ===== Problem #7 =====  */
/*Goal: To create an array of characters. 
  Problem: The original array was too small and needed to be increased by one index to ensure the last line index would have the 
  value of "\0" 
*/
void Problem7(){
	char s[31];
	scanf("%30s",s);
	s[30] = '\0';
}

/* ===== Problem #8 ===== */
/*Goal: To reset the value of x 
  Problem: The initial problem with the code was that it was not actually changing the value of x, rather
  it was just changing the value of the pointer, which doesn't make a difference as it will go out of scope
  and won't affect the main method. 
*/
void reset(int *x){
	*x=0;
}

int main(){
	...
	int x=1;
	reset(&x);
	printf("x is now 0.\n");
	return 0;
}

/* ===== Problem #9 =====  */
/*Goal: To print out "Hello, World!" through the use of the char array s. 
  Problem: The original code did not assign memory to s. Now the modified code copies the first half (Hello) and concatenates the second part of the code (World!). You should also free memory at the end. 
*/
void Problem9(){
    char *s = (char *)malloc(sizeof(s) * 100);
    ...
    strcpy(s, "Hello, ");
    strcat(s, "World!");
    printf("%s\n", s);
    free(s);
}

/* ===== Problem #10 =====  */
/*Goal: To create an array of floats that hold the values of i/10, as i increases from 0 to 9. 
  Problem: The initial code was not allocating memory, now it successfully doing so. You should also free the memory at the end 
  with free(values); 
*/
void Problem10(){
	float *values = malloc(10 * sizeof(float));
	int i,n = 10;
	for(i=0;i<n;i++)
		values[i] = (float)i/n;
}

/* ===== Problem #11 ===== */
/*Goal: To create a multiplication table,  using a two dimensional array. 
  Problem: Since this is a two dimensional array, initially only the first "column" of the array was initialized, now on each of those "cells" another array will be created in the body of the outside loop. 
*/
void Problem11(){
	int **array;
	int i,j;
	array = malloc(10*sizeof(int *));
	for(i=0;i<10;i++){
		array[i]=malloc(10*sizeof(int));
		for(j=0;j<10;j++)
			array[i][j] = i*j;
	}
	
	
	
	/*for(i=0;i<10;i++){
		for(j=0;j<10;j++)
			printf("%d",array[i][j]);
	}
	*/ 
	
	for(i=0;i<10;i++){
		free(array[i]);
	}
	free(array);
	
	
}

/* ===== Problem #12 ===== */
/*Goal: To set values of x if a string was a certain color. 
  Problem: In the original code you were having a string as the case, which is not allowed. This revised method allows you to use
  numbers for each case, by assigning a specific number for each case through the if statements. 
*/
void Problem12(){
	char *str = (char *)malloc(10);
	int x;
	scanf("%9s",str);

	
	int t; 
	if (strcmp(str, "blue") ==0) {t = 1;}
	if (strcmp(str, "red")==0) {t = 2;}
	switch(t) {
		case 1:
			x=1; break;
		case 2:
			x=2; break;
		default: x=0;break;
	}
	
}

/* ===== Problem #13 ===== */ 
/*Goal: To call a specific function when a certain integer was the input. 
  Problem: There was no break statement at the end of each case, without a break statement, the default statement is always 
  printed out.
*/
void Problem13(){
	int i;
	...
	scanf("%d",&i);
	switch(i) {
		case 1: function1();break;
		case 2: function2();break;
		default: printf("Unknown command.\n"); break;
	}
}

/* ===== Problem #14 ===== */
/*Goal: To print out the value of a^b. 
  Problem: The current code is using "cout", which is C++ syntax. Assuming we are importing math.h, using the math library
  the following code should work.The only change was using the printf statement. 
*/
void Problem14(){
    int a,b,c;
    ...
    c = pow(a,b);
    printf("pow(%d,%d) = %d", a,b,c);
}


/* ===== Problem #15 ===== */
/*Goal: To end out of the if statement after a line was printed. 
  Problem: The original code forgot to include the parenthesis, the correct exit statement is exit(1). This means that there
  was a failure and has to exit. 
*/
void Problem15(){
	int x;
	...
	if(x < 0){
		printf("invalid value.\n");
		exit(1);
	}
}

/* ===== Problem #16 ===== */
/*Goal: To create a character array and store the letters abcd in it. 
  Problem: The array did not have enough space for the \0 to be included, signifying the end of the character array. 
*/
void Problem16(){
	char str[5];
	strcpy(str, "abcd");
}


/* ===== Problem #17 ===== */
/*Goal: To print out the radius of a circle, given d, presumably the diameter. 
  Problem: Initially the code used integer division and now the quotient from the division will be a float after it has been 
  casted. 
*/
void Problem17(int d){
    printf("The radius of the circle is %f.\n", (float) d/2);
}

/* ===== Problem #18 ===== */
/*Goal: To return the squared value of the integer in the parameter
  Problem: Simply return the multiplication of the number multiplied by itself, without storing it in the variable sent in. 
*/
int Problem18(const int k){
   return k * k;
	
}


/*
[Part II]
Each question asks you to accomplish a common C task by only using a subset of
C operators.  To receive credit for the question, you must provide working,
compilable C code to each question under the following constraints:
- You must only use the operators specified in each problem.
- You must NOT use any other operators, control statements (if, do/while, ...),
  or function calls.
- You may only use constant values from 0L to 255L (0x0L to 0xFFL).

You know only the following about the machine that is running your code:
- The machine uses 2's complement representation of integers.
- The machine performs right shift arithmetically
- The machine has unpredictable behavior when shifting a long integer by
  more than the word size.

You do not know the size of a word (32-bit, 64-bit, or even 256-bit) on
the machine running your code.  (We want generic code that works on
any machine, not a "fill all the bits of 1's and add one"-type solution.)
*/

/* ===== Problem #19 ===== */
/* Task: Return "value" where all bit positions that are set (==1) in both
         "flag" and "value" are cleared (==0) in the returned result.  All
		 other bits in "value" remain the same.
		 
		 Example: clear_bit(value=0xFF, flag=0x55) => 0xAA

   Allowed Operators: & ~ ^ | >> << ! + =
   (Parenthesis are okay too)
*/
long int clear_bit(long int value, long int flag){
	
	return value & (~flag);

}


/* ===== Problem #20 ===== */
/* Task: Return 1L if (x == y), otherwise return 0L.

   Allowed Operators: & ~ ^ | >> << ! + =
   (Parenthesis are okay too)
*/
long int is_equal(long int x, long int y){
	return !(x^y); 
}
