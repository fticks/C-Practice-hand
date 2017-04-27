#!/bin/bash

# clean
bash clean.sh

# make 
make -C apue.3e/lib/ clean
make -C apue.3e/lib/

cd unpv13e/
./configure
cd ..
make -C unpv13e/lib/ clean
make -C unpv13e/libfree/ clean
make -C unpv13e/lib/
make -C unpv13e/libfree/

cd unpv22e/
./configure
sed -i '/uint.*_t/'d config.h
cd ..
make -C unpv22e/lib/ clean
make -C unpv22e/lib/

# cp to ./
cp apue.3e/include/apue.h apue.3e/lib/libapue.a .

cp unpv13e/libunp.a unpv13e/lib/unp* .
rm -rf *.lh
cp unpv13e/config.h ./unp-config.h

cp unpv22e/libunpipc.a unpv22e/lib/unpipc.h .
cp unpv22e/config.h ./unpipc-config.h

# modify config.h'name 
sed -i 's/\.\.\///g' unp.h
sed -i 's/\.\.\///g' unpipc.h

sed -i 's/config.h/unp-config.h/g' unp.h
sed -i 's/config.h/unpipc-config.h/g' unpipc.h

# cp to ../include
cp -rf libapue.a libunp.a libunpipc.a ../lib/
cp -rf unp-config.h unpipc-config.h  ../include/
cp -rf apue.h unp.h unpipc.h  ../include/

# clean, after install
bash clean.sh

# make clean for all lib
make -C apue.3e/lib/ clean
make -C unpv13e/lib/ clean
make -C unpv13e/libfree/ clean
rm -rf unpv13e/*.a
rm -rf unpv13e/config.log
make -C unpv22e/lib/ clean
rm -rf unpv22e/*.a
