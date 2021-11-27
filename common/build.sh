set -e
name=$1
warning_flags="-Wall -Wextra -Wconversion -Wshadow -Wpadded -Werror"
size_flags="-ffunction-sections -fdata-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--build-id=none"
hc_flags="-static -fno-pie -nostdlib -nostartfiles -ffreestanding"
debug_flags="$warning_flags $size_flags $hc_flags -fsanitize-undefined-trap-on-error -fsanitize=undefined -g -O2 -o $name.bin"
release_flags="$warning_flags $size_flags $hc_flags -Ddebug_NDEBUG -s -O2 -o release_$name.bin"
CC=${CC:-gcc}
$CC $debug_flags main.c
$CC $release_flags main.c
strip --remove-section .comment --remove-section .note.gnu.property release_$name.bin