set -e
CC=${CC:-gcc}
$CC -fsanitize=undefined -fsanitize=address -g -Wall -Wpedantic -Wextra -Wconversion -o gen.bin main.c