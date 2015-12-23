#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *offset_str = "chongo <Landon Curt Noll> /\\../\\";
const unsigned int fnv_prime = (1 << 24) + (1 << 8) + 0x93;

static unsigned int compute_offset_basis()
{
    int i;
    unsigned int offset_basis = 0;
    int str_len = strlen(offset_str);
    for (i = 0; i < str_len; ++i) {
        offset_basis *= fnv_prime;
        offset_basis ^= (offset_str[i] & 0xff);
    }

    return offset_basis;        
}

unsigned int hash_fnv1a_32(char *hash_buf, int length)
{
    unsigned int offset_basis = compute_offset_basis();
    int i;

    for (i = 0; i < length; ++i) {
        offset_basis ^= (hash_buf[i] & 0xff);
        offset_basis *= fnv_prime; 
    }

    return offset_basis;
}

/*
int main(int argc, char **argv)
{
    char *str = "H";
    int len = strlen(str);
       
    printf("FNV-1A Hash Test\n\n");
    printf("fnv_prime = %u or %x\n", fnv_prime, fnv_prime);
    printf("offset_basis = %u or %x\n", compute_offset_basis(),
        compute_offset_basis());
    
    printf("Hash for \"%s\" is %x\n", str, hash_fnv1a_32(str, len));
    return 0;
}
*/
