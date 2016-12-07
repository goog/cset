#ifndef JHASH_H
#define JHASH_H


#define HASH_KEY_LENGTH 512
#define HASH_BUCKET_NUM 1000


typedef struct bucket
{
    int type;     // kv type
    int int_key;
    int int_value;

    char key[HASH_KEY_LENGTH];
    char *value;    // string value pointer

    int use_flag;    // 0 not using
    struct bucket *zipper_head;
} bucket_s;


typedef struct HASH
{
    bucket_s buckets[HASH_BUCKET_NUM];
    int stored_cnt;
} hash_s;


extern hash_s *hash_init(void);
#endif
