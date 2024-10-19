#include<stdio.h>

//function prototypes
int my_strlen(char str[]);
void my_strcpy(char[], char[]);

//function implementations
int my_strlen(char str[])
{
	int length = 0;
	while(str[length] != '\0')
	{
		length++;
	}
	return length;
}

//attempt 1
/*char[] my_strcpy(char s[])
{
	int length = my_strlen(s);
	char dest[length+1];

	for(int i=0;i<length; i++)
	{
		dest[i] = s[i];
	}
	dest[length] = '\0';
	return dest;
}*/

//attempt 2
/*char* my_strcpy(char s[])
{
	int length = my_strlen(s);
	char dest[length+1];

	for(int i=0;i<length; i++)
	{
		dest[i] = s[i];
	}
	dest[length] = '\0';
	return dest;
}*/

//correct implementation with output parameters
void my_strcpy(char src[], char dest[])
{
	int length = my_strlen(src);

	for(int i=0; i<length; i++)
	{
		dest[i] = src[i];
	}
	dest[length] = '\0';
}

//main
int main()
{
	char s[] = "bowser";
	int length = my_strlen(s);
	printf("length is %d\n", length);

	char cpy[length+1];
	my_strcpy(s, cpy);

	char t[] = "peach";
	char cpy2[my_strlen(t)+1];
	my_strcpy(t, cpy2);

	printf("copy is %s\n", cpy);
	printf("copy 2 is %s\n", cpy2);
	return 0;
}
