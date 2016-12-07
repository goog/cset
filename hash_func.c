//#TODO replace hash functions later

long int hash_int(long int x)
{
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x);
	return x;
}

// http://www.cse.yorku.ca/~oz/hash.html
// // hash function for string keys djb2
long int hash_string(char * str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}
