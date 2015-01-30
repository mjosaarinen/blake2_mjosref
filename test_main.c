// test_main.c
// 24-Jan-15  Markku-Juhani O. Saarinen <mjos@iki.fi>

#include <stdio.h>

#include "blake2b.h"
#include "blake2s.h"

// Deterministic sequences (Fibonacci generator)

static void selftest_seq(uint8_t *out, size_t len, uint32_t seed)
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

// BLAKE2b self-test validation. Return 0 when OK.

int blake2b_selftest()
{
    // grand hash of hash results
    const uint8_t blake2b_res[32] = {
        0x22, 0x71, 0x94, 0x24, 0x17, 0xFB, 0x9B, 0x58,
        0xFC, 0xA6, 0x43, 0x85, 0xE2, 0x62, 0x25, 0xD3,
        0x70, 0x44, 0x13, 0x82, 0x94, 0x7F, 0x4C, 0x6B,
        0xD9, 0x42, 0x40, 0xAA, 0x2B, 0x7E, 0xC7, 0x05
    };
    // parameter sets
    const size_t b2b_md_len[4] = { 16, 20, 32, 64 };
    const size_t b2b_in_len[6] = { 0, 3, 128, 129, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[64], key[64];
    blake2b_ctx ctx;

    // 256-bit hash for testing
    if (blake2b_init(&ctx, 32, NULL, 0))
        return -1;

    for (i = 0; i < 4; i++) {
        outlen = b2b_md_len[i];
        for (j = 0; j < 6; j++) {
            inlen = b2b_in_len[j];

            selftest_seq(in, inlen, inlen);     // unkeyed hash
            blake2b(md, outlen, NULL, 0, in, inlen);
            blake2b_update(&ctx, md, outlen);   // hash the hash

            selftest_seq(key, outlen, outlen);  // keyed hash
            blake2b(md, outlen, key, outlen, in, inlen);
            blake2b_update(&ctx, md, outlen);   // hash the hash
        }
    }

    // compute and compare the hash of hashes
    blake2b_final(&ctx, md);
    for (i = 0; i < 32; i++) {
        if (md[i] != blake2b_res[i])
            return -1;
    }

    return 0;
}

// BLAKE2s self-test validation. Return 0 when OK.

int blake2s_selftest()
{
    // grand hash of hash results
    const uint8_t blake2s_res[32] = {
        0x9D, 0x6C, 0x34, 0x4A, 0x25, 0x42, 0xF0, 0x57,
        0xC4, 0x88, 0x29, 0x36, 0x71, 0xC9, 0xCE, 0x36,
        0x3E, 0xAA, 0xB2, 0x57, 0x07, 0x81, 0xF9, 0xCE,
        0x88, 0x70, 0xF9, 0xA3, 0xA0, 0x35, 0xBF, 0x35
    };
    // parameter sets
    const size_t b2s_md_len[4] = { 16, 20, 24, 32 };
    const size_t b2s_in_len[6] = { 0,  3,  64, 65, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[32], key[32];
    blake2s_ctx ctx;

    // 256-bit hash for testing
    if (blake2s_init(&ctx, 32, NULL, 0))
        return -1;

    for (i = 0; i < 4; i++) {
        outlen = b2s_md_len[i];
        for (j = 0; j < 6; j++) {
            inlen = b2s_in_len[j];

            selftest_seq(in, inlen, inlen);     // unkeyed hash
            blake2s(md, outlen, NULL, 0, in, inlen);
            blake2s_update(&ctx, md, outlen);   // hash the hash

            selftest_seq(key, outlen, outlen);  // keyed hash
            blake2s(md, outlen, key, outlen, in, inlen);
            blake2s_update(&ctx, md, outlen);   // hash the hash
        }
    }

    // compute and compare the hash of hashes
    blake2s_final(&ctx, md);
    for (i = 0; i < 32; i++) {
        if (md[i] != blake2s_res[i])
            return -1;
    }

    return 0;
}


// test driver

int main(int argc, char **argv)
{
    printf("blake2b_selftest() = %s\n",
         blake2b_selftest() ? "FAIL" : "OK");
    printf("blake2s_selftest() = %s\n",
         blake2s_selftest() ? "FAIL" : "OK");

    return 0;
}

