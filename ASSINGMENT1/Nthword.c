//-----------------------------------------
// NAME: your name 
// STUDENT NUMBER: 7980132
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Saulo Lectures 
// ASSIGNMENT: assignment 1, QUESTION: question 1
// 
// REMARKS: extract the nth word from text file.
//
//-----------------------------------------

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
    printf("\nProgram completed normally.\n");
    return 0;
}


//------------------------------------------------------
// extractWord
//
// PURPOSE: extracts nth word for each line.
// INPUT PARAMETERS:
// theLine - The line to extract word from.
// word - the nth value
// result - result to be updated with the nth word
// OUTPUT PARAMETERS:
// result - the extracted word.
//------------------------------------------------------
void extractWord(char *theLine, int word, char *result) {
    int start;
    int end;
    int spaces = 1;
    int lineLength = strlen(theLine);

     // search for the nth non-blank character
    for(start = 0; start < lineLength && spaces < word; start++) {
        if(isspace(theLine[start]) != 0) {
            spaces++;
        }
    }
    
// only need to continue if we haven't gone past the end of the string
    if(start < lineLength) {
        // the next blank character is the end
        for(end = start; end < lineLength && isspace(theLine[end]) == 0; end++);

        // we now have the word 
        strncpy(result, theLine + start, end);
        result[end - start] = '\0';
    }
}

