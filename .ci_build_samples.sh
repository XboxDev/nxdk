#!/bin/sh
set -e

if [ $(uname) = 'Darwin' ]; then
    NUMCORES=$(sysctl -n hw.logicalcpu)
else
    NUMCORES=$(nproc)
fi

for dir in samples/*/
do
    cd "$dir"
    make -j${NUMCORES}
    cd ../..
done
