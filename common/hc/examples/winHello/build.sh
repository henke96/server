#!/bin/sh
set -e
script_dir="$(dirname $0)"
flags="-Wl,-subsystem:console -O2 -s $FLAGS"
"$script_dir/../../cc_pe.sh" $flags -S -o "$script_dir/release.exe.s" "$script_dir/main.c"
"$script_dir/../../cc_pe.sh" $flags -o "$script_dir/release.exe" "$script_dir/main.c" -lkernel32 -luser32
