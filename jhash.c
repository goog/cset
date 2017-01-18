#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "jhash.h"
#include "hash_func.h"



#define DEBUG 1


hash_s *hash_init(void)
{
    hash_s *h = (hash_s *)calloc(sizeof(hash_s), 1);
    
    return h;
}



void free_hash_bucket(bucket_s *bucket)
{
    if(bucket != NULL)
    {
        if(bucket->value != NULL)
        {
            free(bucket->value);
        }

        free(bucket);
    }

}


#if 0
int hash_insert_strstr_kv(hash_s *hash, const char *key, const char *value)
{

    if(key == NULL || value == NULL)
    {
        fprintf(stderr, "insert parameter is NULL.\n");
        return -1;
    }

    long int index;
    char *strkey_p = NULL;
    bucket_s *first_bucket = NULL;

    
    index = hash_string(key) % HASH_BUCKET_NUM;
    //strkey_p = key;
    first_bucket = &(hash->buckets[index]);

    if(first_bucket->use_flag == 0 || strcmp(first_bucket->key, key) == 0)
    {
        // first bucket for one index unused or have the same key then overwrite it
        strncpy(first_bucket->key, key, strlen(key));
            
        if(first_bucket->value) free(first_bucket->value);

        first_bucket->value = strdup(value);
        first_bucket->use_flag = 1;

    }
    else
    {
        // key collision and handle key is the same
        bucket_s *new_bucket;
        bucket_s *p = first_bucket->zipper_head;
        bucket_s *cur = NULL;

        while(p != NULL)
        {
            
            cur = p;

            if(strcmp(cur->key, key) == 0)
            {
                if(cur->value != NULL) free(cur->value);

                cur->value = strdup(value);
                return 0;
            }

            p = p->zipper_head;
        }
            
        

        new_bucket = (bucket_s *)calloc(sizeof(bucket_s), 1);
        if(new_bucket == NULL)
	{
            fprintf(stderr, "calloc failed.\n");
            return -1;
	}
	    
	

        strncpy(new_bucket->key, key, strlen(key));
        new_bucket->value = strdup(value);
	

	// add link to new element
	p->zipper_head = new_bucket;

    }

}
#endif



// modify @20170110
int hash_insert_strint_kv(hash_s *hash, const char *key, int value)
{
    if(key == NULL)
    {
        fprintf(stderr, "parameter key pointer is NULL at %s.\n", __FUNCTION__);
        return -1;
    }

    long int index;
    bucket_s *first_bucket = NULL;

    
    index = hash_string(key) % HASH_BUCKET_NUM;
    
    // the first element on list is statically allocated.
    first_bucket = &(hash->buckets[index]);

    if(first_bucket->use_flag == 0)
    {
        // unused
        strncpy(first_bucket->key, key, strlen(key));
        first_bucket->int_value = value;
        
        first_bucket->use_flag = 1;
    }
    else if(strcmp(first_bucket->key, key) == 0)
    {
        //have the same key then overwrite it
        first_bucket->int_value = value;
    }
    else
    {
        // key collision

        bucket_s *new_bucket;
        bucket_s *curr = first_bucket->next;
        

        while(curr != NULL)
        {
            //next = cur->zipper_head;

            if(strcmp(curr->key, key) == 0)
            {
                curr->int_value = value;
                return 0;
            }

            curr = curr->next;
        }
        
        // not found in list
        new_bucket = (bucket_s *)calloc(sizeof(bucket_s), 1);
        if(new_bucket == NULL)
	{
            fprintf(stderr, "calloc failed.\n");
            return -1;
	}
	
        strncpy(new_bucket->key, key, strlen(key));
        new_bucket->int_value = value;
	
	curr->next = new_bucket;

    }

}


// return INT_MIN when can't or error
int hash_get_intvalue_by_str(hash_s *hash, char *key)
{
    long int index;
    index = hash_string(key) % HASH_BUCKET_NUM;

    bucket_s *curr = &(hash->buckets[index]);

    while(strcmp(curr->key, key))
    {
        curr = curr->next;
        
        if(curr == NULL)
        {
            return INT_MIN;
        }
    }

    if(curr->use_flag != -1)    //not the deleted node
        return curr->int_value;

    return INT_MIN;
}


#if 0
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
#endif


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
    current = current->next;

    while(current != NULL && strcmp(current->key, key))
    {
        prev = current; // keep previous element
        current = current->next;
        
        if(current == NULL)
            return -1;

    }

    prev->next = current->next;
    
    free_hash_bucket(current);

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

        if(cur->value != NULL)
            free(cur->value);
        
        
        cur = cur->next;
        while(cur)
        {
            bucket_s *curr_copy = cur;
            cur = cur->next;

                
            if(curr_copy->value !=NULL) free(curr_copy->value);
            
            free(curr_copy);
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
