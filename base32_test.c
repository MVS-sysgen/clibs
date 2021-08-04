#include "base32.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    const char *base32_value;
    int result_avail, i, retval, rtn;
    uint8_t *result1, *result2;
    rtn = 0; // return code

    if( argc > 2 ) {
        printf("Too many arguments supplied.\n");
        printf("Usage: %s [string]\n\n", argv[0]);
        return 1337;
    }

   if( argc == 2 ) {

       // First we decode the Base32 string

    base32_value = argv[1];
   } else {
     base32_value = "JBSWY3DPEHPK3PXP";
   }

    result_avail = strlen(base32_value) + 1;
    printf("Base32 : %s\n", base32_value);

    if (result_avail < strlen(base32_value)) {
      printf("Integer overflow in input size");
      exit(1);
    }

    result1 = malloc(result_avail);

    retval = base32_decode((uint8_t *)base32_value, result1, result_avail);

    if (retval < 0) {
      fprintf(stderr, "base32_decode failed.  Input too long?\n");
      exit(1);
    }

    printf("Decoded: 0x");
    for(i=0;i<retval;++i)
            printf("%02x", result1[i]);
    putchar('\n');
    //full_write(STDOUT_FILENO, result, retval);

    // Then we re-encode it and compare results
    result_avail = ((retval + 4) / 5 * 8 + 6 + 1);
    result2 = malloc(result_avail);
    retval = base32_encode(result1, retval, result2, result_avail);
    printf("Base32 : ");
    for(i=0;i<retval;++i)
            printf("%c", result2[i]);
    putchar('\n');

    if( memcmp(base32_value, result2, strlen(base32_value)) == 0) {
        printf("TEST   : PASS\n");
    } else {
        printf("TEST   : FAIL\n");
        rtn = 1;
    }

    free(result1);
    free(result2);

   return rtn;
  }
