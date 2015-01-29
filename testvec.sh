#!/bin/bash
if ./xb2test | cmp testvec.txt -
then echo "BLAKE2 test OK"
else echo "BLAKE2 test FAIL."
fi

