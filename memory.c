#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef SYSTEM_MALLOC
#define xmalloc malloc
#define xfree free
#else
#include "malloc.h"
#endif

int rand_between(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

#define TEST_SIZE 10

#define MIN(a,b) ( (a) < (b) ? (a) : (b) )

int main(int argc, char* argv[])
{
    //srand(2315);
    srand(time(NULL));
    char* test_string = "Now is the time for all the good people to come to the aid of their country.";

    char *ptrs[TEST_SIZE];

    printf("--------------Allocating--------------\n\n");

    for (int i = 0; i < TEST_SIZE; i++)
    {
        int size = rand_between(1,50);
        fprintf(stderr,"[%d] size: %d, ", i, size);

        ptrs[i] = xmalloc(size);

        if (ptrs[i] == NULL)
        {
            printf("[%d] malloc failed\n", i);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr,"[%d] ptrs[%d]: %p, going to copy %d chars\n",
            i, i, ptrs[i], len_to_copy);

        strncpy(ptrs[i], test_string, len_to_copy);
        ptrs[i][len_to_copy] = '\0';

        fprintf(stderr,"[%x] %s\n\n", i, ptrs[i]);
    }

    printf("--------------Freeing--------------\n\n");

    for(int i = 0; i < TEST_SIZE; i++)
    {
        fprintf(stderr,"[%d] freeing %p (%s), ", i, ptrs[i], ptrs[i]);
        xfree(ptrs[i]);
        fprintf(stderr,"[%d] freed %p\n\n", i, ptrs[i]);
    }

    return 0;
}

