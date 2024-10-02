#include<stdio.h>

void totalDistance(char *s, char *t, int expected);
int levenshtein(char *s, char *t);

void totalDistance(char *s, char *t, int expected) {
    int distance = levenshtein(s, t);

    if(distance == expected) {
        printf("Passed! You can get from '%s' to '%s' in %d moves.", s, t, expected);
    } else {
        printf("FAILED: I thought it would take %d moves, but apparently its takes %d moves to get from %s to %s.", epected, distance, s, t);
        testsFailed++;
    }
}

int levenshtein(char *s, char *t) {
    int cost; 
    int distance;
    
}
