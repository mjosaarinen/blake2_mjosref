// blake2b.h
// BLAKE2b Prototypes and Hashing Context

#ifndef BLAKE2B_H
#define BLAKE2B_H

#include <stdint.h>
#include <stddef.h>

// state context
typedef struct {
    uint8_t b[128];                     // input buffer
    uint64_t h[16];                     // chained state
    uint64_t t[2];                      // total # of bytes
    size_t c;                           // pointer for buffer
    size_t outlen;                      // digest size
} blake2b_ctx;

// Initialize the hashing context "ctx" with optional key "key".
//      1 <= outlen <= 64 gives the digest size in bytes.
//      Secret key is optional (keylen = 0).

int blake2b_init(blake2b_ctx *ctx, size_t outlen,
    const void *key, size_t keylen);    // Secret key

// Add "inlen" bytes from "in" into the hash
void blake2b_update(blake2b_ctx *ctx,   // Context
    const void *in, size_t inlen);      // Data to be hashed

// Generate the message digest (size given in init).
//      Result placed in "out"
void blake2b_final(blake2b_ctx *ctx, void *out);

// All-in-one convenience function.
int blake2b(void *out, size_t outlen,   // Return buffer for digest
    const void *key, size_t keylen,     // Optional secret key
    const void *in, size_t inlen);      // Data to be hashed
#endif
