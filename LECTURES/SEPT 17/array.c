#include <stdio.h>

int main() {
    #define ARR_LENGTH 10
    int arr[ARR_LENGTH] = {0}; // Replaces all index values with zero
    int x = 5;

    for(int i = -1; i <= ARR_LENGTH; i++) {
        printf("%d\n", arr[i]);
    }

    return 0;
}