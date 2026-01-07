#include "shell.h"

/**
 * Gets length of a char array
 */
int getArrLength(char **arr)
{
    char *el;
    int i = 0;
    while((el = arr[i])){
        i++;
    }
    return i;
}