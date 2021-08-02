#include <stdio.h>
#include <string.h>
#include <sha1.h>
#ifdef JCC
#include <sockets.h> // needed for ASCII to EBCDIC conversion
#endif

int sha1_test()
{
	char *text1;
	char *text2;
	char *text3;

	uint8_t hash1[SHA1_DIGEST_SIZE] = {0xa9,0x99,0x3e,0x36,0x47,0x06,0x81,0x6a,0xba,0x3e,0x25,0x71,0x78,0x50,0xc2,0x6c,0x9c,0xd0,0xd8,0x9d};
	uint8_t hash2[SHA1_DIGEST_SIZE] = {0x84,0x98,0x3e,0x44,0x1c,0x3b,0xd2,0x6e,0xba,0xae,0x4a,0xa1,0xf9,0x51,0x29,0xe5,0xe5,0x46,0x70,0xf1};
	uint8_t hash3[SHA1_DIGEST_SIZE] = {0x34,0xaa,0x97,0x3c,0xd4,0xc4,0xda,0xa4,0xf6,0x1e,0xeb,0x2b,0xdb,0xad,0x27,0x31,0x65,0x34,0x01,0x6f};
    uint8_t results[20];

    char *buf;
    int idx, i, n;
	int pass = 1;

	SHA1_CTX ctx;

    text1 = "abc";
    text2 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    text3 = "aaaaaaaaaa";

    printf("                                 SHA-1 Tests\n");

    for(i=0; i < 79; ++i)
        putchar('-');

    printf("\n\nSHA-1 Tests:\n 1 - %s\n 2 - %s\n 3 - %s (100,000 rounds)\n\n", text1, text2, text3);

#ifdef JCC
    // Since we're hashing EBCDIC strings
    // It's easier to convert and then compare
    // and check against ASCII systems
    ebcdic2ascii (text1, strlen(text1));
    ebcdic2ascii (text2, strlen(text2));
    ebcdic2ascii (text3, strlen(text3));
#endif

    printf("Hashing: 0x");
    for(n=0; n < strlen(text1); ++n)
        printf("%02x", text1[n]);
    putchar('\n');

	SHA1_Init(&ctx);
	SHA1_Update(&ctx, (uint8_t *)text1, strlen(text1));
	SHA1_Final(results, &ctx);
	pass = pass && !memcmp(hash1, results, SHA1_DIGEST_SIZE);

    printf("Hex Digest: ");
    for (n = 0; n < 20; n++)
            printf("%02x", results[n]);
    putchar('\n');
    printf("Expected:   ");
    for (n = 0; n < 20; n++)
            printf("%02x", hash1[n]);
    putchar('\n');
    putchar('\n');

    printf("Hashing: 0x");
    for(n=0; n < strlen(text2); ++n)
        printf("%02x", text2[n]);
    putchar('\n');

	SHA1_Init(&ctx);
	SHA1_Update(&ctx, (uint8_t *)text2, strlen(text2));
	SHA1_Final(results, &ctx);
    pass = pass && !memcmp(hash2, results, SHA1_DIGEST_SIZE);

    printf("Hex Digest: ");
    for (n = 0; n < 20; n++)
            printf("%02x", results[n]);
    putchar('\n');
    printf("Expected:   ");
    for (n = 0; n < 20; n++)
            printf("%02x", hash2[n]);
    putchar('\n');
    putchar('\n');

    printf("Hashing: 0x");
    for(n=0; n < strlen(text3); ++n)
        printf("%02x", text3[n]);
    putchar('\n');

	SHA1_Init(&ctx);
	for (idx = 0; idx < 100000; ++idx)
	   SHA1_Update(&ctx, text3, strlen(text3));
	SHA1_Final(results, &ctx);
	pass = pass && !memcmp(hash3, results, SHA1_DIGEST_SIZE);

    printf("Hex Digest: ");
    for (n = 0; n < 20; n++)
            printf("%02x", results[n]);
    putchar('\n');
    printf("Expected:   ");
    for (n = 0; n < 20; n++)
            printf("%02x", hash3[n]);
    putchar('\n');
    putchar('\n');

    for(i=0; i < 79; ++i)
        putchar('-');
    putchar('\n');
    putchar('\n');

	return(pass);
}

int main(int argc, char* argv[]) {
    SHA1_CTX sha;
    uint8_t results[20];
    char *buf;
    int n;
    char zOut[44];
    int i;
    static const char zEncode[] = "0123456789abcdef";
    buf = "xyz123";

    if( argc > 2 ) {
        printf("Too many arguments supplied.\n");
        printf("Usage: %s [string to convert to ASCII and hash]\n\n", argv[0]);
        return 1337;
    }

   if( argc == 2 ) {
        printf("\nString argument: %s\n", argv[1]);
        buf = argv[1];


#ifdef JCC
        printf("Converting to ASCII\nFrom: 0x");
        for(n=0; n < strlen(buf); ++n)
            printf("%02x", buf[n]);

        // To compare with Linux results
        // Convert string from EBCDIC to ASCII

        ebcdic2ascii (buf, strlen(buf));

        printf(" To: 0x");
        for(n=0; n < strlen(buf); ++n)
            printf("%02x", buf[n]);


        putchar('\n');
#endif

        printf("Hashing: 0x");
        for(n=0; n < strlen(buf); ++n)
            printf("%02x", buf[n]);
        putchar('\n');

        n = strlen(buf);
        SHA1_Init(&sha);
        SHA1_Update(&sha, (uint8_t *)buf, n);
        SHA1_Final(results, &sha);



        /* Print the digest as one long hex value */
        printf("Hex Digest:    ");
        for (n = 0; n < 20; n++)
                printf("%02x", results[n]);
        putchar('\n');

        /* If you need/want to store the hash as a string */
        /* Convert the digest to a string */
        for(i=0; i<20; i++){

            zOut[i*2] = zEncode[(results[i]>>4)&0xf];
            zOut[i*2+1] = zEncode[results[i] & 0xf];
        }
        zOut[i*2]= 0;
        /* Print the digest as a string */
        printf("String Digest: %s\n", zOut);

        printf("\n");

   } // End of argument hashing
   else
   {
        printf("SHA-1 tests: %s\n\n", sha1_test() ? "SUCCEEDED" : "FAILED");
   }

return 0;
}

