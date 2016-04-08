#ifndef SIMPLE_STRING_SET_H_
#define SIMPLE_STRING_SET_H_


#define ELEMENTS_NUMBER  20
#define ELEMENT_LEN  256

typedef struct
{
    char elements[ELEMENTS_NUMBER][ELEMENT_LEN];
    int size;
}Stringset_s;

extern Stringset_s *string_set_init();
extern void string_set_memset(Stringset_s * p_set);
extern int string_set_size(Stringset_s * p_set);
extern int string_set_add(Stringset_s * p_set, const char *str);
extern int string_set_delete(Stringset_s * p_set, const char *str);
extern int string_set_intersection(Stringset_s * p_set_a, Stringset_s * p_set_b, Stringset_s * p_set_c);
extern int string_set_diff(Stringset_s * p_set_a, Stringset_s * p_set_b, Stringset_s * p_set_c);


#endif
