#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


//DON'T DO THIS
int main(void)
{
    FILE *f;
    unsigned int count = 0;
    while (true)
    {
        f = fopen("text.txt", "r");
        if (f != NULL)
        {
            ftell(f);
            count++;
            if (count % 1000 == 0)
            {
                printf("Opened %d files.\n", count);
            }
        }
        else
        {
            printf("Failed at %d files.\n", count);
            exit(1);
        }
    }
}
