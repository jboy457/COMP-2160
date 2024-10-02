#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

// Prototype for extractWord 
void extractWord(char *theLine, int word, char *result);

int main() {
    #define LINE_SIZE 500

    // Line Container
    char buffer[LINE_SIZE];
    char word[LINE_SIZE];
    
    while (fgets(buffer, LINE_SIZE, stdin) != NULL) {
        // read "word" from file
        fgets(word, LINE_SIZE, stdin);

        // add terminators to read string 
        buffer[strlen(buffer) - 1] = '\0';
        word[strlen(word) - 1] = '\0';
    
        printf("Read line: \"%s\", extracting word [%s]\n", buffer, word);

        char result[LINE_SIZE];
        int numWord = atoi(word);
        extractWord(buffer, numWord, result);
        printf("Word #%d is: %s\n", numWord, result);
    }
    printf("\nEnd of processing\n");
    return 0;
}

void extractWord(char *theLine, int word, char *result) {
    int start;
    int end;
    int spaces = 1;

    for(start = 0; start < strlen(theLine) && spaces < word; start++) {
        if(isspace(theLine[start]) != 0) {
            spaces++;
        }
    }

    if(start < strlen(theLine)) {
        for(end = start; end < strlen(theLine) && isspace(theLine[end]) == 0; end++);
        strncpy(result, theLine + start, end);
        result[end - start] = '\0';
    }
}

