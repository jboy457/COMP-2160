#include <stdio.h>

int main()
{
	int age;
	int ret;
	float money;
	char name[4];

	printf("Enter your name, then age, then money\n");
	ret = scanf(" %s %d %f", name, &age, &money);

	if(!ret)
	{
		printf("wrong format\n");
	}
	else if(ret == EOF)
	{
		printf("\nend of file, age=%d, money=%f\n", age, money);
	}
	else
	{
		printf("You entered name = %s, age = %d, and money = %f\n", name, age, money);
		for(int i=0; i<45; i++)
		{
			printf("%d -> %d %c\n", i, name[i], name[i]);
		}
	}

	return 0;
}
