//-----------------------------------------
// NAME: Adejare Taiwo
// STUDENT NUMBER: 7980132
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Saulo Lectures 
// ASSIGNMENT: assignment 1, QUESTION: question 2
// 
// REMARKS: Levnshtein 
//
//-----------------------------------------

#include<stdio.h>
#include<string.h>
#include<assert.h>

// Prototype for all declared function
void testDistance(char *s, char *t, int expected);
int levenshtein(char *s, char *t);
int minimum(int minimum[], int size);

// Declare global for test results
int testsFailed = 0;
int testsExecuted = 0;

int main() {
    printf( "Testing typical cases.\n" );
    testDistance( "kitten", "kitten", 0 );
    testDistance( "kitten", "sitting", 3 );
    testDistance( "kitten", "mittens", 2 );
    testDistance( "balloon", "saloon", 2 );
    testDistance( "hello", "doggo", 4 );
    testDistance( "\t\thi", "\t\t\t\thi", 2 );

    printf( "\n\nTesting empty/edge cases.\n" );
    testDistance( "", "", 0 );
    testDistance( "hello", "", 5 );
    testDistance( "", "doggo", 5 );
    testDistance( "a", "b", 1 );
    testDistance( "b", "b", 0 );
    testDistance( " ", " ", 0 );

    printf( "\nThis might take a while...\n" );
    testDistance( "12345678901", "123456789012", 1 );   // how many chars are we looking at?

    printf( "\n******These tests will be opposite in the C version******\n" );
    printf( "\n******These tests **should** FAIL in the C version*******\n" );
    testDistance( "kitten", "mitten\0s", 3 );           // ????
    testDistance( "\0totally", "\0different", 9 );

    printf( "\n\nTotal number of tests executed: %d", testsExecuted );
    printf( "\nNumber of tests passed: %d", (testsExecuted - testsFailed) );
    printf( "\nNumber of tests failed: %d\n", testsFailed );
}

//------------------------------------------------------
// testDistance
// PURPOSE: Compares the distance returned by the levenshtein function with the expected distance
//          Prints a success or failure message based on the comparison
// INPUT PARAMETERS: s, t, expected distance
//------------------------------------------------------
void testDistance(char *s, char *t, int expected) {
    int distance = levenshtein(s, t);

    // Print the result of the test
    if(distance == expected) {
        printf("\nPassed! You can get from '%s' to '%s' in %d moves.", s, t, expected);
    } else {
        printf("\nFAILED: I thought it would take %d moves, but apparently its takes %d moves to get from '%s' to '%s'.", expected, distance, s, t);
        testsFailed++;
    }

    testsExecuted++;
}

//------------------------------------------------------
// levenshtein
//
// PURPOSE: This function calculates the distance between two strings
// INPUT PARAMETERS:
// s: The first string
// t: The second string
// OUTPUT PARAMETERS:
// The  function returns the distance between the two strings
//------------------------------------------------------
int levenshtein(char *s, char *t) {
    int sLength = strlen(s);
    int tLength = strlen(t);

    int cost; 
    int distance = 0;
    char deleteS[sLength];
    char deleteT[tLength];

    if(sLength == 0) {
        distance = tLength;
    } else if(tLength == 0) {
        distance = sLength;
    } else {
        if(s[0] == t[0]) {
            cost = 0;
        } else {
            cost = 1;
        }

        // Copy the strings form first index fo s and t to deleteS and deleteT
        strcpy(deleteS, s+1);
        strcpy(deleteT, t+1);

        int deleteSLength = strlen(deleteS);
        int deleteTLength = strlen(deleteT);

        // Check if the substrings are correct
        assert(deleteSLength  == sLength - 1);
        assert(deleteTLength == tLength - 1);

        // Check if the s ends with delete values
        assert(strncmp(s + sLength - deleteSLength, deleteS, deleteSLength) == 0);
        assert(strncmp(t + tLength - deleteTLength, deleteT, deleteTLength) == 0);

        int values[3] = {levenshtein(deleteS, t) + 1, levenshtein(s, deleteT) + 1, levenshtein(deleteS, deleteT) + cost};
        distance = minimum(values, 3);
    }
    return distance;
}

//------------------------------------------------------
// minimum
//
// PURPOSE: This function returns the smalest value from teh minimums array 
// INPUT PARAMETERS:
// minimums - Array of integers
// size: Thie number of elements in the minimums array
// OUTPUT PARAMETERS:
// The  function returns teh smallest integer value in minimums
//------------------------------------------------------
int minimum(int minimums[], int size) {
    int min = 0;
    assert(size > 0);
    min = minimums[0];

    for(int i = 1; i < size; i++) {
        if(minimums[i] < min) {
            min = minimums[i];
        }
    }
     return min;

}