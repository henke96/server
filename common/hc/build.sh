set -e
compile_flags="-Wall -Wextra -Wconversion -Wshadow -Wpadded -Werror -fno-pie -nostdlib -ffreestanding -fno-asynchronous-unwind-tables"
link_flags="-nostartfiles -Wl,--gc-sections -Wl,--build-id=none -static"
debug_compile_flags="$compile_flags -fsanitize-undefined-trap-on-error -fsanitize=undefined -g -O2 $CFLAGS"
debug_link_flags="$link_flags $LFLAGS"
release_compile_flags="$compile_flags -Ddebug_NDEBUG -O2 $CFLAGS"
release_link_flags="$link_flags -s $LFLAGS"
CC=${CC:-gcc}
$CC $debug_compile_flags -S -o debug.bin.s main.c
$CC $debug_compile_flags -c -o debug.bin.o main.c
$CC $debug_link_flags -o debug.bin debug.bin.o
$CC $release_compile_flags -S -o release.bin.s main.c
$CC $release_compile_flags -c -o release.bin.o main.c
$CC $release_link_flags -o release.bin release.bin.o