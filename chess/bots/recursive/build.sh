set -e

name="recursive"
common_flags="-Wpedantic -Wall -Wextra -Wconversion -Wno-unused-parameter -Wno-implicit-fallthrough"
CC=${CC:-gcc}
$CC -fsanitize=undefined -fsanitize=address -g -o debug_$name.bin $common_flags $name.c
$CC -static -g -o $name.bin $common_flags $name.c
$CC -static -s -O2 -DNDEBUG -DUNREACHABLE='__builtin_unreachable()' -o release_$name.bin $common_flags $name.c
echo "Build success"