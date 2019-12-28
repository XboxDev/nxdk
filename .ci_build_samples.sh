#!/bin/sh
set -e

if [ $(uname) = 'Darwin' ]; then
    NUMCORES=$(sysctl -n hw.logicalcpu)
else
    NUMCORES=$(nproc)
fi

cd samples
for dir in */
do
    cd "$dir"
    make -j${NUMCORES}
    cd ..
done
