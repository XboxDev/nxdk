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

echo "::group::Building nxdk"
make NXDK_ONLY=y -j${NUMCORES}
echo "::endgroup::"

for dir in samples/*/
do
    echo "::group::Building $dir"
    cd "$dir"
    make -j${NUMCORES}
    cd ../..
    echo "::endgroup::"
done
