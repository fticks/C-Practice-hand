#!/bin/bash

rm -rf *.h *.a

# make clean for all lib
make -C apue.3e/lib/ clean
make -C unpv13e/lib/ clean
make -C unpv13e/libfree/ clean
rm -rf unpv13e/*.a
rm -rf unpv13e/config.log                                                   
make -C unpv22e/lib/ clean
rm -rf unpv22e/*.a
