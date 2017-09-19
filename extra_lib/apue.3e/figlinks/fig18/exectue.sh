#!/bin/bash

make clean
make
echo -e 
#./fig18.22
#./lserial
./test_uart1
#./ex3.3
echo -e 
make clean

