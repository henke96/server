#!/bin/sh
set -e
script_dir="$(dirname $0)"
flags="$(cat $script_dir/flags)"
CC="${CC:-clang}"
LD="${LD:-lld}"
ARCH="${ARCH:-x86_64}"
"$CC" $flags -target $ARCH-unknown-linux-elf -fuse-ld="$LD" -Wl,--gc-sections -Wl,--build-id=none "$@"
