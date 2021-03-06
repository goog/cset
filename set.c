#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "jhash.h"

#define PRESENT 16


typedef struct HASH set_s;


set_s *set_init()
{
    return hash_init();
}


void free_set(set_s *s)
{
    free_hash(s);
}


int set_add(set_s *set, char *key)
{
    return hash_insert_strint_kv(set, key, PRESENT);
}


int set_del(set_s *set, char *key)
{
    return hash_del_strkey(set, key);
}



set_s *set_intersection(set_s *a, set_s *b)
{
    int i;
    set_s *result = set_init();
    if(result == NULL)
        return NULL;

    bucket_s *buck = NULL;

    for(i = 0; i < HASH_BUCKET_NUM; i++)
    {
        buck = &(a->buckets[i]);

        while(buck != NULL)
        {

            if(buck->use_flag != -1 && hash_get_intvalue_by_str(b, buck->key) != INT_MIN)
            {
                set_add(result, buck->key);
            }
            
            buck = buck->next;
        }     

    }
    
    return result;
}


set_s *set_diff(set_s *a, set_s *b)
{

    int i;
    set_s *result = set_init();
    if(result == NULL)
        return NULL;

    bucket_s *buck = NULL;

    for(i = 0; i < HASH_BUCKET_NUM; i++)
    {
        buck = &(a->buckets[i]);

        while(buck != NULL)
        {

            if(buck->use_flag != -1 && hash_get_intvalue_by_str(b, buck->key) == INT_MIN)
            {
                set_add(result, buck->key);
            }
            
            buck = buck->next;
        }        

    }
    
    return result;
}


void print_set(set_s *set)
{
    if(set == NULL)
        return;
    
    int i;
    bucket_s *buck = NULL;
    
    printf("print set elements:\n");
    for(i = 0; i < HASH_BUCKET_NUM; i++)
    {
        buck = &(set->buckets[i]);

        while(buck != NULL)
        {

            if(buck->use_flag != -1 && strlen(buck->key) != 0)
            {
                printf("set element: %s.\n", buck->key);
            }
            
            buck = buck->next;
        }        

    }

}


int main()
{
    set_s *s = set_init();
    

    printf("begin to insert.\n");
    set_add(s, "abc");
    printf("after first add.\n");

    set_add(s, "bcd");
    set_add(s, "ttttt");
    set_add(s, "kkk");

    printf("there should be 4 elements.\n");
    print_set(s);

    set_del(s, "kkk");
    printf("there should be 3 elements.\n");
    print_set(s);

}
