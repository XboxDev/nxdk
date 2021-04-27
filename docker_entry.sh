#!/usr/bin/env sh
set -e

eval $(/usr/src/nxdk/bin/activate -s)

exec "$@"
