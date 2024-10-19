#include <stdio.h>

int main()
{
	FILE* f = fopen("output.txt", "w"); //will create output.txt if it doesn't exist
	fprintf(f, "it's the same!\n");
	fclose(f);
	return 0;
}
