#!/usr/bin/bash

set -euo pipefail

this_file="${BASH_SOURCE[0]}"
this_dir="$(dirname "$this_file")"

root="$(readlink -m "$this_dir/..")"
build_dir="$root/_build"

mkdir -p "$build_dir"

EARTHLY_URL="https://github.com/earthly/earthly/releases/latest/download/earthly-linux-amd64"

earthly_bin="$build_dir/earthly"

if test -f "$earthly_bin"; then
    # Do nothing
    true
elif type -P wget > /dev/null; then
    wget "$EARTHLY_URL" -qO "$earthly_bin"
else
    curl "$EARTHLY_URL" -sLo "$earthly_bin"
fi

chmod a+x "$earthly_bin"

command "$earthly_bin" "$@"
