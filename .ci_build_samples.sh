#!/bin/sh

# SPDX-License-Identifier: CC0-1.0

# SPDX-FileCopyrightText: 2018-2026 Stefan Schmidt
# SPDX-FileCopyrightText: 2021 Margen67

set -e

eval $(./bin/activate -s)

NUMCORES=$(nxdk-nproc)

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
