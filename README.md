# MVS C Library

Below are C libraries to be used with MVS 3.8j and the JCC compiler. Each comes with an example/test C program and instructions to compile and link on MVS.

To compile/use these libraries you need:
- **rdrprep** https://github.com/mvslovers/rdrprep and
- **jcc** https://github.com/mvslovers/jcc

Install **rdrprep** on your machine then clone jcc to this folder (`git clone https://github.com/mvslovers/jcc.git`) to follow the examples. You will also need to change the socket reader from `ASCII` to `EBCDIC`:

In the hercules console issue the commands:
* `detach c`
* `attach c 3505 3505 sockdev ebcdic trunc eof`

This change wont be permanent so you will need to do it every time you open hercules.

## Auto install

To automatically install the test programs you can run `./install.sh`

## SHA-1

Generates a SHA-1 hash.

Files:
- `sha1.c` the SHA-1 library
- `sha1.h` the SHA-1 header
- `sha1_test.c` SHA-1 test program

To install:
1. Compile `sha1.c` and `sha1_test.c` with jcc:
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ -D_BIG_ENDIAN sha1.c`
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ sha1_test.c`
1. Prelink the object together with jcc (`sha1.load` is the output from this command):
    1. `./jcc/prelink -test=file -r ./jcc/objs sha1.load sha1.obj sha1_test.obj`
1. Create object cards in EBCDIC with `rdrprep` (the output of this is `reader.jcl`):
    1. `rdrprep sha1_link.jcl.template`
1. Submit this JCL to MVS 3.8j (replace localhost with your hercules IP address if on a different machine):
    1. `cat reader.jcl | ncat --send-only -w1 localhost 3505`

`SHA1` should now be installed in `SYS2.CMDLIB` and can be tested with `SHA1` or `SHA1 stringyouwanttohash`

## HMAC

Generates an HMAC SHA-1 digest.

Files:
- `hmac_sha1.c` the HMAC library
- `hmac.h` the HMAC header
- `hmac_test.c` SHA-1 test program

To install:
1. Compile `hmac_sha1.c`, `sha1.c`, and `sha1_test.c` with jcc:
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ hmac_sha1.c`
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ -D_BIG_ENDIAN sha1.c`
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ hmac_test.c`
1. Prelink the object together with jcc (`hmac.load` is the output from this command):
    1. `./jcc/prelink -test=file -r ./jcc/objs hmac.load hmac_sha1.obj sha1.obj hmac_test.obj`
1. Create object cards in EBCDIC with `rdrprep` (the output of this is `reader.jcl`):
    1. `rdrprep hmac_link.jcl.template`
1. Submit this JCL to MVS 3.8j (replace `localhost` with your hercules IP address if on a different machine):
    1. `cat reader.jcl | ncat --send-only -w1 localhost 3505`

`HMAC` should now be installed in `SYS2.CMDLIB` and can be tested with `HMAC` or `HMAC KEY MESSAGE`

## Base32

Encode and Decodes Base32.

Files:
- `base32.c` base32 library
- `base32.h` the base32 header
- `base32_test.c` base32 test program

To install:
1. Compile `base32.c` and `base32_test.c`
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ base32.c`
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ base32_test.c`
1. Prelink the object together with jcc (`b32test.load` is the output from this command):
    1. `./jcc/prelink -test=file -r ./jcc/objs b32test.load base32.obj base32_test.obj`
1. Create object cards in EBCDIC with `rdrprep` (the output of this is `reader.jcl`):
    1. `rdrprep base32_link.jcl.template`
1. Submit this JCL to MVS 3.8j (replace `localhost` with your hercules IP address if on a different machine):
    1. `cat reader.jcl | ncat --send-only -w1 localhost 3505`

`BASE32` should now be installed in `SYS2.CMDLIB` and can be tested with `BASE32` or `BASE32 [base32 string]` e.g. `BASE32 JVLFGNCFKZASC===`

## TOTP

Generates Time based One Time Pass (TOTP)

Works with Google Authenticator

Files:
- `totp_test.c` base32 library

To install:
1. Compile `base32.c`, `sha1.c`, `hmac_sha1.c`, and `totp_test.c`
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ base32.c`
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ hmac_sha1.c`
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ -D_BIG_ENDIAN sha1.c`
    1. `./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ totp_test.c`
1. Prelink the object together with jcc (`totptest.load` is the output from this command):
    1. `./jcc/prelink -test=file -r ./jcc/objs totptest.load base32.obj hmac_sha1.obj  sha1.obj  totp_test.obj`
1. Create object cards in EBCDIC with `rdrprep` (the output of this is `reader.jcl`):
    1. `rdrprep totp_link.jcl.template`
1. Submit this JCL to MVS 3.8j (replace `localhost` with your hercules IP address if on a different machine):
    1. `cat reader.jcl | ncat --send-only -w1 localhost 3505`

`TOTPTEST` should now be installed in `SYS2.CMDLIB` and can be tested with `TOTPTEST` or `TOTPTEST [secret key]` e.g. `TOTPTEST JBSWY3DPEHPK3PXX`