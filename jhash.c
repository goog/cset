#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#include "jhash.h"
#define DEBUG 1



hash_s *hash_init(void)
{
    hash_s *h = (hash_s *)calloc(sizeof(hash_s), 1);
    #if DEBUG
    printf("hash pointer %p.\n", h);
    #endif
    return h;
}



/*        |key    |value |
 * -----------------------            
 * type 0  string string
 * type 1  string int
 * type 2  int    int
 * type 3  int    string
 *
 *
 *
 *
 *
 */

void free_hash_bucket(bucket_s *bucket)
{
    if(bucket != NULL)
    {
        if(bucket->value != NULL)
            free(bucket->value);

        free(bucket);
    }


}


int hash_tail_insert(bucket_s *tail, int type, void *key, void *value)
{
    bucket_s *new_bucket = NULL;

    new_bucket = (bucket_s *)calloc(sizeof(bucket_s), 1);
    if(new_bucket == NULL)
    {
        fprintf(stderr, "calloc failed.\n");
        return -1;
    }
    
    new_bucket->type = type;
    if(type == 0)
    {
        strncpy(new_bucket->key, (char *)key, strlen((char *)key));
        new_bucket->value = strdup((char *)value);
    }
    else if(type == 1)
    {
        strncpy(new_bucket->key, (char *)key, strlen((char *)key));
        new_bucket->int_value = *(int *)value;
    }

    // add link to new element
    tail->zipper_head = new_bucket;

    return 0;
}



int hash_insert(hash_s *hash, int type, void *key, void *value)
{
    if(key == NULL || value == NULL)
    {
        fprintf(stderr, "insert parameter error.\n");
        return -1;
    }

    long int index;
    char *strkey_p = NULL;
    bucket_s *first_bucket = NULL;

    if(type == 0)
    {
        index = hash_string(key) % HASH_BUCKET_NUM;
        strkey_p = (char *)key;
        first_bucket = &(hash->buckets[index]);  // node before zipper

        if(first_bucket->use_flag == 0 || strcmp(first_bucket->key, (char *)key) == 0)
        {
            // unused or have the same key then overwrite it
            first_bucket->type = 0;
            strncpy(first_bucket->key, strkey_p, strlen(strkey_p));
            
            if(first_bucket->value) free(first_bucket->value);

            first_bucket->value = strdup((char *)value);
            first_bucket->use_flag = 1;

        }
        else
        {
            // key collision and handle key is the same
            bucket_s *new_bucket;
            bucket_s *next = first_bucket;
            bucket_s *cur = NULL;

            while(next->zipper_head != NULL)
            {
                // if key is same, overwrite
                cur = next->zipper_head;

                if(strcmp(cur->key, (char *)key) == 0)
                {
                    if(cur->value != NULL) free(cur->value);

                    cur->value = strdup((char *)value);
                    return 0;
                }

                next = next->zipper_head;
            }
            
            hash_tail_insert(next, 0, key, value);

        }


    }
    else if(type == 1)
    {
        first_bucket = NULL;
        #if DEBUG
        printf("type: %d.\n", type);
        printf("begin insert at function %s.\n", __FUNCTION__);
        #endif
        index = hash_string(key) % HASH_BUCKET_NUM;
        printf("index: %ld.\n", index);

        strkey_p = (char *)key;
        printf("the key is %s.\n", strkey_p);
        first_bucket = &(hash->buckets[index]);  // node before zipper
        #if DEBUG
        printf("first bucket %p.\n", first_bucket);
        #endif

        //if(first_bucket->use_flag == 0 || strcmp(first_bucket->key, (char *)key) == 0)
        
        printf("the use_flag is %d.\n", first_bucket->use_flag);
        
        if(first_bucket->use_flag == 0)
        {
            // unused or have the same key then overwrite it
            #if DEBUG
            printf("write the first bucket %s.\n", __FUNCTION__);
            #endif
            first_bucket->type = 1;
            strncpy(first_bucket->key, strkey_p, strlen(strkey_p));

            first_bucket->int_value = *(int *)value;
            first_bucket->use_flag = 1;

        }
        #if 0
        else
        {
            printf("type 1 key collision.\n");
            // key collision and handle key is the same
            bucket_s *new_bucket;
            bucket_s *next = first_bucket;
            bucket_s *cur = NULL;

            while(next->zipper_head != NULL)
            {
                // if key is same, overwrite
                cur = next->zipper_head;

                if(strcmp(cur->key, (char *)key) == 0)
                {
                    //if(cur->value != NULL) free(cur->value);
                    cur->int_value = *(int *)value;
                    return 0;
                }

                next = next->zipper_head;
            }
            
            hash_tail_insert(next, 1, key, value);

        }
        #endif

        #if DEBUG
        //printf("type: %d.\n", type);
        printf("end insert at function %s.\n", __FUNCTION__);
        #endif

    }
    else if(type == 2)  // ...
    {


    }





}


// return INT_MIN when can't or error
int hash_get_intvalue_by_str(hash_s *hash, char *key)
{
    long int index;
    index = hash_string(key) % HASH_BUCKET_NUM;

    bucket_s *next = &(hash->buckets[index]);
    #if 0
    if(next->use_flag == 0)     // THIS CHAIN is empty
    {
        return INT_MIN;
    }
    #endif

    while(strcmp(next->key, key))
    {
        next = next->zipper_head;
        if(next == NULL)
            return INT_MIN;
    }

    if(next->use_flag != -1)    
        return next->int_value;

    return INT_MIN;
}



const char *hash_get_strvalue_by_str(hash_s *hash, char *key)
{
    long int index;
    index = hash_string(key) % HASH_BUCKET_NUM;
    
    #if DEBUG
    printf("index : %ld \n", index);
    #endif

    bucket_s *next = &(hash->buckets[index]);
   
    #if 0 
    if(next->use_flag == 0)     // THIS CHAIN is empty
    {
        return NULL;
    }
    #endif
    

    while(strcmp(next->key, key) != 0)
    {
        next = next->zipper_head;
        if(next == NULL)
            return NULL;
    }

    #if DEBUG
    printf("the next use_flag %d at %s.\n", next->use_flag, __FUNCTION__);
    #endif
    if(next->use_flag != -1)
        return next->value;
    
    return NULL;
}


int hash_del_strkey(hash_s *hash, const char *key)
{
    long int index;
    index = hash_string(key) % HASH_BUCKET_NUM;

    bucket_s *current = &(hash->buckets[index]);
    if(strcmp(current->key, key) == 0)
    {
        hash->buckets[index].use_flag = -1;  // deleted flag
        return 0;
    }
    
    bucket_s *prev = current;
    bucket_s *next = current->zipper_head;

    while(next != NULL && strcmp(next->key, key))
    {
        prev = next; // keep previous element
        next = next->zipper_head;
        if(next == NULL)
            return -1;

    }

    prev->zipper_head = next->zipper_head;

    //if(next->value) free(next->value);
    //if(next) free(next);
    free_hash_bucket(next);

    return 0;
}



int hash_del_intkey(hash_s *hash, int key)
{





}




void free_hash(hash_s *h)
{
    if(h == NULL)
        return;
    

    int i = 0;
    bucket_s *cur = NULL;

    for(i = 0; i < HASH_BUCKET_NUM; i++)
    {

        cur = &(h->buckets[i]);

        if(cur->type == 0)
        {
            if(cur->value != NULL)
                free(cur->value);
        }
        
        if(cur->zipper_head != NULL)
        {

            bucket_s *next = NULL;
            next = cur->zipper_head;
            #if DEBUG
            printf("before next while line %d.\n", __LINE__);
            #endif
            while(next)
            {
                bucket_s *temp = next;
                next = next->zipper_head;
                
                if(temp->type == 0)
                {
                    free_hash_bucket(temp);
                }
                else
                {
                    free(temp);
                }


                #if DEBUG
                printf("one loop ends.\n");
                #endif
            }

        }

        

    }

    free(h);
}


#if 0
int main()
{

    hash_s *h = hash_init();

    // str -> str
    hash_insert(h, 0, "abc", "dddiii");
    hash_insert(h, 0, "a", "ddd");
    hash_insert(h, 0, "b", "ddd");
    hash_insert(h, 0, "c", "ddd");
    hash_insert(h, 1, "ll", (int []) { 5 });

    const char *str_value;

    str_value = hash_get_strvalue_by_str(h, "abc");
    printf("the string value is %s.\n", str_value);

    hash_del_strkey(h, "abc");
    printf("after delete key.\n");

    str_value = hash_get_strvalue_by_str(h, "abc");
    printf("the string value is %s.\n", str_value);
    
    int acont = 666;
    hash_insert(h, 1, "h4h", &acont);
    int value = hash_get_intvalue_by_str(h, "h4h");
    printf("the int value : %d.\n", value);
    value = hash_get_intvalue_by_str(h, "ll");
    printf("the int value : %d.\n", value);

    free_hash(h);
}
#endif
