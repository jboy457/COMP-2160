#include <stdio.h>

int main()
{
	int x = 1;
	int y = 2;
	int z = 3;

	int *a, *b;

	a = &y;
	b = &x;
	*b = z;
	x = *a;

	printf("x = %d\n", x);
	printf("y = %d\n", y);
	printf("z = %d\n", z);

	printf("address of x is %p\n", &x);
	printf("address of y is %p\n", &y);

	printf("a = %p\n", a);
	printf("b = %p\n", b);

	return 0;
}
