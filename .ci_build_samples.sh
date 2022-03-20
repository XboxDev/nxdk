#!/bin/sh

# SPDX-License-Identifier: CC0-1.0

# SPDX-FileCopyrightText: 2018-2021 Stefan Schmidt
# SPDX-FileCopyrightText: 2021 Margen67

set -e

eval $(./bin/activate -s)

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
