#include <stdio.h>

#define LENGTH 100

//struct
struct STRING
{
	char contents[LENGTH];
	int length;
};

//function prototypes
struct STRING make_string(char*);
char char_at(struct STRING str, int pos);

//function implementations
struct STRING make_string(char *string)
{
	struct STRING my_string;
	int i=0;

	while(string[i] != '\0')
	{
		my_string.contents[i] = string[i];
		i++;
	}
	my_string.contents[i] = '\0';
	my_string.length = i;

	return my_string;
}

char char_at(struct STRING str, int pos)
{
	if(pos >= str.length)
	{
		return 0;
	}
	else
	{
		return str.contents[pos];
	}
}

int main()
{
	struct STRING my_string;
	my_string = make_string("metronome");

	printf("string %s has length %d\n", my_string.contents, my_string.length);
	printf("position %d is %c\n", 8, char_at(my_string, 8));
	printf("position %d is %c\n", 10, char_at(my_string, 10));

	return 0;
}
