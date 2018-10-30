#!/bin/sh

set -e

cd samples

for dir in */
do
    cd "$dir"
    make
    cd ..
done

