#include <stdio.h>

void swap(int *x, int *y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

int main()
{
	int a = 4;
	int b = 5;

	printf("initially, a = %d, b = %d\n", a, b);
	swap(&a, &b);
	printf("after swap, a = %d, b = %d\n", a, b);

	return 0;
}
