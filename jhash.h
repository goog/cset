#ifndef JHASH_H
#define JHASH_H


#define HASH_KEY_LENGTH 512
#define HASH_BUCKET_NUM 1000


typedef struct bucket
{
    int int_key;
    int int_value;

    char key[HASH_KEY_LENGTH];
    char *value;    // string value pointer

    int use_flag;    // 0 not using
    struct bucket *next;
} bucket_s;


typedef struct HASH
{
    bucket_s buckets[HASH_BUCKET_NUM];
    int stored_cnt;
} hash_s;


hash_s *hash_init(void);
void free_hash(hash_s *hash);
int hash_insert_strstr_kv(hash_s *hash, const char *key, const char *value);
int hash_insert_strint_kv(hash_s *hash, const char *key, int value);
int hash_del_strkey(hash_s *hash, const char *key);
int hash_get_intvalue_by_str(hash_s *hash, char *key);

#endif
