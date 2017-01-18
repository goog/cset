#include <stdio.h>
#include <string.h>
#include "xxhash.h"


long int hash_int(long int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x);
    
    return x;
}


unsigned long long hash_string(const char * str)
{
    #if 0
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    #endif
    //printf("%s begin.\n", __FUNCTION__);
    unsigned long long hash = XXH64(str, strlen(str), 0);

    //printf("xxhash value is %llu.\n", hash);
    
    return hash;
}
