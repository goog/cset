#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_string_set.h"


Stringset_s *string_set_init()
{

    Stringset_s * p_set = (Stringset_s *)calloc(1, sizeof(Stringset_s));
    if(p_set == NULL)
    {
        printf("memory calloc error\n");
    }

    return p_set;
}

void string_set_memset(Stringset_s * p_set)
{

    if(p_set != NULL)
    {
        memset(p_set, 0, sizeof(Stringset_s));
    }

}


int string_set_size(Stringset_s * p_set)
{
    if(p_set)
    {
        return p_set->size;
    }

    return 0;
}

// every element is different
int string_set_add(Stringset_s *p_set, const char *str)
{
    if(p_set == NULL)
    {
        printf("input in null set pointer.\n");
        return 0;
    }

    int size = string_set_size(p_set);

    if(size == ELEMENTS_NUMBER)
    {
        printf("string set is full.\n");
        return -1;
    }

    
    snprintf(p_set->elements[size], ELEMENT_LEN, "%s", str);

    p_set->size++;
}


// delete one element
int string_set_delete(Stringset_s *p_set, const char *str)
{
    int i, index = -1;

    if(p_set == NULL)
    {
        printf("input in null set pointer.\n");
        return -1;
    }

    int size = string_set_size(p_set);
    printf("<begin of string_set_delete> the size is %d.\n", size);
    printf("the string is %s.\n", str);

    for(i = 0; i < size; i++)
    {
        if(strcmp(p_set->elements[i], str) == 0)
        {
            index = i;
            break;
        }
    }

    // found it
    if(index >= 0)
    {
        printf("we got the to delete element.\n");
        for(i = index; i < size -1; i++)
        {
            memset(p_set->elements[i], 0, ELEMENT_LEN);
            strncpy(p_set->elements[i], p_set->elements[i+1], strlen(p_set->elements[i+1]));
        }

        memset(p_set->elements[size - 1], 0, ELEMENT_LEN);

        p_set->size--;
    }

    printf("end of delete set, size is %d.\n", p_set->size);
    
    return 0;
}


int string_set_intersection(Stringset_s * p_set_a, Stringset_s * p_set_b, Stringset_s * p_set_c)
{
    int i, j, k = 0;

    int set_a_size = string_set_size(p_set_a);
    int set_b_size = string_set_size(p_set_b);

    for(i=0; i < set_a_size; i++)
    for(j=0; j < set_b_size; j++)
    {
        if(strcmp(p_set_a->elements[i], p_set_b->elements[j]) == 0)
        {
            strncpy(p_set_c->elements[k++], p_set_a->elements[i], strlen(p_set_a->elements[i]));
        }


    }

    p_set_c->size = k;

}


int string_set_diff(Stringset_s * p_set_a, Stringset_s * p_set_b, Stringset_s * p_set_c)
{
    int i, j, k = 0;

    int set_a_size = string_set_size(p_set_a);
    int set_b_size = string_set_size(p_set_b);

    for(i=0; i < set_a_size; i++)
    {
        for(j=0; j < set_b_size; j++)
        {
            if(strcmp(p_set_a->elements[i], p_set_b->elements[j]) == 0)
            {

                break;
            }


        }

        if(j == set_b_size)
        {

            strncpy(p_set_c->elements[k++], p_set_a->elements[i], strlen(p_set_a->elements[i]));
        }



    }


    p_set_c->size = k;

}
