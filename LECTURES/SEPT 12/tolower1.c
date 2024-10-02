#include <stdio.h>
#include <ctype.h>

int main(void) {
    int nextchar;

    // Get the first character of stdin 
    nextchar = getchar();

    // While it hasnt reach end of line  print the lower case
    while(nextchar != EOF) {
        printf("%c", nextchar);
        // tolower coverts to lower case 
        // putchar print the caracter into standart ouput(terminal or file); 
        putchar(tolower(nextchar));
        nextchar = getchar();
    }

    return 0;
}