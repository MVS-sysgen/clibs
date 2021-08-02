/* hmac_test for jcc/MVS */

/* Take two arguments data and key and computes hmac
   requires sha1.c */

#include <stdio.h>
#include <string.h>
#include <hmac.h>

int main(int argc, char *argv[]) {
    char *sec_key;
    char *data;
    char out[256] = {0};
    unsigned int len = sizeof(out);
    int i;

    if( argc == 3 ) {
         sec_key = argv[1];
         data = argv[2];
    } else {
         sec_key= "abc";
         data = "abc";
    }

    // printf("Key: %s\nMsg: %s\n", sec_key, data);

    hmac_sha1(sec_key, strlen(sec_key), data, strlen(data), out, &len);

    printf("len: %d\n", len);

    for(i=0;i < len; ++i)
        printf("%02x", out[i] & 0xff);
    putchar('\n');

#ifdef JCC
    // Since we're hashing EBCDIC strings
    // It's easier to convert and then compare
    // and check against ASCII systems
    printf("MVS System Converting to ASCII\n");
    ebcdic2ascii (sec_key, strlen(sec_key));
    ebcdic2ascii (data, strlen(data));
    hmac_sha1(sec_key, strlen(sec_key), data, strlen(data), out, &len);
    printf("len: %d\n", len);
    for(i=0;i < len; ++i)
        printf("%02x", out[i] & 0xff);
    putchar('\n');
#endif


    return 0;
}
