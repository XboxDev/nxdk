#!/usr/bin/env bash

set -eu
set -o pipefail

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
readonly SCRIPT_DIR

NXDK_ROOT_DIR="$( realpath ${SCRIPT_DIR}/../.. )"
BIN_DIR="${NXDK_ROOT_DIR}/bin"
readonly BIN_DIR
SHARE_DIR="${NXDK_ROOT_DIR}/share"
readonly SHARE_DIR

echo "Verify that building works without the NXDK_DIR environment variable set."
TOOLCHAIN_FILE="${SHARE_DIR}/toolchain-nxdk.cmake"
unset NXDK_DIR
pushd test_project &> /dev/null
rm -rf build-noenvvar &> /dev/null
cmake -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" -S . -B build-noenvvar
cmake --build build-noenvvar
rm -rf build-noenvvar &> /dev/null
popd &> /dev/null
echo "Build without env-var completed!"


NXDK_CMAKE="${BIN_DIR}/nxdk-cmake"
readonly NXDK_CMAKE

echo "Verify that building works with the NXDK_DIR environment variable set."
pushd "${BIN_DIR}" &> /dev/null
. activate -s
popd &> /dev/null

pushd test_project &> /dev/null
rm -rf build-envvar &> /dev/null
"${NXDK_CMAKE}" -S . -B build-envvar
cmake --build build-envvar
rm -rf build-envvar &> /dev/null
popd &> /dev/null

echo "Build with env-var completed!"
