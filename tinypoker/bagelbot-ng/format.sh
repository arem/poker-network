#!/bin/sh

find ./ -name \*.[ch] -exec indent -nbad -nbfda -bap -nbc -br -brs -c33 \
-cd33 -ncdb -ce -ci4 -cli0 -cp33 -d0 -di1 -nfc1 -nfca -i8 -ip0 -l240    \
-lp -npcs -npsl -nsc -nsob -nss -ts8 -cs -ut {} \;
find ./ -name \*.[ch]~ -exec rm {} \;
