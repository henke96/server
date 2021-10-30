set -e

(cd ../../common/htmlPacker && ./build.sh)
(cd client && ../../../common/htmlPacker/htmlPacker.bin html/main.html generatedHtml)
echo "Client build success"

name="server"
common_flags="-Wpedantic -Wall -Wextra -Wconversion -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-language-extension-token"
CC=${CC:-gcc}
$CC -static -nostdlib -nostartfiles -ffreestanding -g -o $name.bin $common_flags $name.c
$CC -static -nostdlib -nostartfiles -ffreestanding -Ddebug_NDEBUG -s -O2 -o release_$name.bin $common_flags $name.c
$CC -Dnolibc_YESLIBC -fsanitize=undefined -fsanitize=address -g -o debug_$name.bin $common_flags $name.c
echo "Server build success"