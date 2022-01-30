set -e
name=$1
compile_flags="-Wall -Wextra -Wconversion -Wshadow -Wpadded -Werror -fno-pie -nostdlib -ffreestanding -fno-asynchronous-unwind-tables"
link_flags="-nostartfiles -Wl,--gc-sections -Wl,--build-id=none -static"
debug_compile_flags="$compile_flags -fsanitize-undefined-trap-on-error -fsanitize=undefined -g -O2 $CFLAGS"
debug_link_flags="$link_flags $LFLAGS"
release_compile_flags="$compile_flags -Ddebug_NDEBUG -O2 $CFLAGS"
release_link_flags="$link_flags -s $LFLAGS"
CC=${CC:-gcc}
$CC $debug_compile_flags -S -o $name.bin.s main.c
$CC $debug_compile_flags -c -o $name.bin.o main.c
$CC $debug_link_flags -o $name.bin $name.bin.o
$CC $release_compile_flags -S -o release_$name.bin.s main.c
$CC $release_compile_flags -c -o release_$name.bin.o main.c
$CC $release_link_flags -o release_$name.bin release_$name.bin.o