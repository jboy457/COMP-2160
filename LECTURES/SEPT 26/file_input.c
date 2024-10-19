#include <stdio.h>
#include <string.h>

#define LINE_SIZE 80

int main()
{
	FILE* f = fopen("text.txt", "r");
	char line[LINE_SIZE];
	int last_char_index;

	if(f != NULL)
	{
		while(fgets(line, LINE_SIZE, f) != NULL)
		{
			last_char_index = strlen(line) - 1;
			if(line[last_char_index] == '\n')
			{
				line[last_char_index] = '\0';
			}
		printf("line is %s\n", line);
		}
		fclose(f);
	}
	else
	{
		printf("file does not exist\n");
	}
	return 0;
}
