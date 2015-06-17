// test_main.c
// Self test Modules for BLAKE2b and BLAKE2s -- and a stub main().

#include <stdio.h>

#include "blake2b.h"
#include "blake2s.h"

// Deterministic sequences (Fibonacci generator).

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
        0xC2, 0x3A, 0x78, 0x00, 0xD9, 0x81, 0x23, 0xBD,
        0x10, 0xF5, 0x06, 0xC6, 0x1E, 0x29, 0xDA, 0x56,
        0x03, 0xD7, 0x63, 0xB8, 0xBB, 0xAD, 0x2E, 0x73,
        0x7F, 0x5E, 0x76, 0x5A, 0x7B, 0xCC, 0xD4, 0x75
    };
    // parameter sets
    const size_t b2b_md_len[4] = { 20, 32, 48, 64 };
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
    // Grand hash of hash results.
    const uint8_t blake2s_res[32] = {
        0x6A, 0x41, 0x1F, 0x08, 0xCE, 0x25, 0xAD, 0xCD,
        0xFB, 0x02, 0xAB, 0xA6, 0x41, 0x45, 0x1C, 0xEC,
        0x53, 0xC5, 0x98, 0xB2, 0x4F, 0x4F, 0xC7, 0x87,
        0xFB, 0xDC, 0x88, 0x79, 0x7F, 0x4C, 0x1D, 0xFE
    };
    // Parameter sets.
    const size_t b2s_md_len[4] = { 16, 20, 28, 32 };
    const size_t b2s_in_len[6] = { 0,  3,  64, 65, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[32], key[32];
    blake2s_ctx ctx;

    // 256-bit hash for testing.
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

    // Compute and compare the hash of hashes.
    blake2s_final(&ctx, md);
    for (i = 0; i < 32; i++) {
        if (md[i] != blake2s_res[i])
            return -1;
    }

    return 0;
}

// Test driver.

int main(int argc, char **argv)
{
    printf("blake2b_selftest() = %s\n",
         blake2b_selftest() ? "FAIL" : "OK");
    printf("blake2s_selftest() = %s\n",
         blake2s_selftest() ? "FAIL" : "OK");

    return 0;
}

