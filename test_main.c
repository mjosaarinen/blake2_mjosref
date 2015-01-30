// test_main.c
// 24-Jan-15  Markku-Juhani O. Saarinen <mjos@iki.fi>

#include <stdio.h>

#include "blake2b.h"
#include "blake2s.h"

// test driver

int main(int argc, char **argv)
{
    printf("blake2b_selftest() = %s\n",
         blake2b_selftest() ? "FAIL" : "OK");
    printf("blake2s_selftest() = %s\n",
         blake2s_selftest() ? "FAIL" : "OK");

    return 0;
}

