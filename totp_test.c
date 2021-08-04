#include <stdint.h>
#include <base32.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <hmac.h>


#define VERIFICATION_CODE_MODULUS (1000*1000)

int main(int argc, char *argv[]) {
    const char *base32_value;
    int result_avail, i, retval, rtn, msg;
    char offset;
    uint8_t *result1, *result2, *message;
    char out[256] = {0};
    unsigned int len = sizeof(out);
    uint8_t challenge[8];
    unsigned int truncatedHash = 0;

    time_t seconds;

   seconds = time(NULL);
   msg = (int)(seconds/30);

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
    printf("\nBase32 : %s\n", base32_value);

    if (result_avail < strlen(base32_value)) {
      printf("Integer overflow in input size");
      exit(1);
    }

    result1 = malloc(result_avail);

    retval = base32_decode((uint8_t *)base32_value, result1, result_avail);

    if (retval < 0) {
      printf("base32_decode failed.  Input too long?\n");
      exit(1);
    }

    printf("Decoded: 0x");
    for(i=0;i<retval;++i)
            printf("%02x", result1[i]);
    putchar('\n');

    // convert integer to 8 bytes
  for (i = 8; i--; msg >>= 8) {
    challenge[i] = msg;
  }


    hmac_sha1(result1, retval, challenge, 8, out, &len);
    offset = out[19] & 15;

  for (i = 0; i < 4; ++i) {
    truncatedHash <<= 8;
    truncatedHash  |= out[offset + i] & 0xff;
  }

  // Truncate to a smaller number of digits.
  truncatedHash &= 0x7FFFFFFF;
  truncatedHash %= VERIFICATION_CODE_MODULUS;

    printf("%d\n", truncatedHash);

    free(result1);

   return 0;
  }