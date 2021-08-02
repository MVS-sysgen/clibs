#!/bin/bash
set -e
echo "Compiling SHA1"

./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ -D_BIG_ENDIAN sha1.c
./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ sha1_test.c
./jcc/prelink -test=file -r ./jcc/objs sha1.load sha1.obj sha1_test.obj

echo "Compiling HMAC"
./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ hmac_sha1.c
./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ -D_BIG_ENDIAN sha1.c
./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ hmac_test.c
./jcc/prelink -test=file -r ./jcc/objs hmac.load hmac_sha1.obj sha1.obj hmac_test.obj

echo "Compiling BASE32"
./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ base32.c
./jcc/jcc -I./jcc/include -I./ -o -D__MVS_ base32_test.c
./jcc/prelink -test=file -r ./jcc/objs b32test.load base32.obj base32_test.obj

echo "Creating JCL"
cat *.jcl.* > all.template
rdrprep all.template

host=$1
if [[ -z "$1" ]]; then
    host='localhost'
fi
echo "Uploading to $host"
cat reader.jcl | ncat --send-only -w1 $host 3505
rm all.template
echo "SHA1, HMAC and BASE32 commands installed"
echo "Install Completed Succesfully!"