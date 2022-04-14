#!/bin/sh
set -e
script_dir="$(dirname $0)"
flags="-O2 -s $FLAGS"
"$script_dir/../../cc_wasm.sh" $flags -S -o "$script_dir/release.wasm.s" "$script_dir/main.c"
"$script_dir/../../cc_wasm.sh" $flags -o "$script_dir/release.wasm" "$script_dir/main.c"
