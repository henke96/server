#!/bin/sh
set -e
script_dir="$(dirname $0)"
flags="-Wl,-subsystem:efi_application -O2 -s $FLAGS"
"$script_dir/../../cc_pe.sh" $flags -S -o "$script_dir/release.efi.s" "$script_dir/main.c"
"$script_dir/../../cc_pe.sh" $flags -o "$script_dir/release.efi" "$script_dir/main.c"
