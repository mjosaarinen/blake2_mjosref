// main.c
// 24-Jan-15  Markku-Juhani O. Saarinen <mjos@iki.fi>

// This is an "condensed" test driver for the BLAKE2 Algorithms.
// We use a deterministic byte sequence genereator based on the
// Fibonacci generator to create the data and keys for hashing.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blake2b.h"
#include "blake2s.h"

// Deterministic sequences (Fibonacci generator)

void fibofill_seq(uint8_t *out, size_t len, uint32_t seed)
{
    size_t i;
    uint32_t t, a , b;

    a = 0xDEAD4BAD * seed;              // prime
    b = 1;

    for (i = 0; i < len; i++) {         // fill the buf
        t = a + b;
        a = b;
        b = t;
        out[i] = (t >> 24) & 0xFF;
    }
}


// hex output

void hexout(uint8_t *vec, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++) {
        if ((i & 0x1F) == 0)
            printf("\n    ");
        printf("%02X", vec[i]);
    }
    printf("\n");
}

// output test vectors

void blake2b_testvecs()
{
    const size_t b2b_md_len[4] = { 16, 20, 32, 64 };
    const size_t b2b_in_len[6] = { 0,  3,  128, 129, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[64], key[64];


    for (i = 0; i < 4; i++) {
        outlen = b2b_md_len[i];
        for (j = 0; j < 6; j++) {
            inlen = b2b_in_len[j];
            fibofill_seq(in, inlen, inlen);

            printf("BLAKE2b(out=%u in=%u key=%u): ",
                (unsigned) outlen, (unsigned) inlen, 0);
            blake2b(md, outlen, NULL, 0, in, inlen);
            hexout(md, outlen);

            fibofill_seq(key, outlen, outlen);
            printf("BLAKE2b(out=%u in=%u key=%u): ", (unsigned) outlen,
                (unsigned) inlen, (unsigned) outlen);
            blake2b(md, outlen, key, outlen, in, inlen);
            hexout(md, outlen);
        }
    }
}

// output test vectors

void blake2s_testvecs()
{
    const size_t b2s_md_len[4] = { 16, 20, 24, 32 };
    const size_t b2s_in_len[6] = { 0,  3,  128, 129, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[32], key[32];

    for (i = 0; i < 4; i++) {
        outlen = b2s_md_len[i];
        for (j = 0; j < 6; j++) {
            inlen = b2s_in_len[j];
            fibofill_seq(in, inlen, inlen);

            printf("BLAKE2s(out=%u in=%u key=%u): ",
                (unsigned) outlen, (unsigned) inlen, 0);
            blake2s(md, outlen, NULL, 0, in, inlen);
            hexout(md, outlen);

            fibofill_seq(key, outlen, outlen);
            printf("BLAKE2s(out=%u in=%u key=%u): ", (unsigned) outlen,
                (unsigned) inlen, (unsigned) outlen);
            blake2s(md, outlen, key, outlen, in, inlen);
            hexout(md, outlen);
        }
    }
}

// test driver

int main(int argc, char **argv)
{
    blake2b_testvecs();
    blake2s_testvecs();

    return 0;
}

