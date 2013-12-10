#include<stdio.h>

int add(int a, int b)
{
	int c = a + b;
	return c;
}

int mul (int a, int b)
{
	int c = a * b;
	return c;
}


int main()
{
	int a = add(5,6);
	int b = mul(5,6);
	char* p = NULL;
	*p = 1;
	printf("Program executed successfully!\n");
	return 0;
}
