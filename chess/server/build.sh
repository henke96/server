set -e

(cd client && ../../../common/htmlPacker/htmlPacker.bin html/main.html generatedHtml)
if test "$MINIFY" = "yes"; then
html-minifier --collapse-whitespace --remove-comments --remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes --use-short-doctype --minify-css true --minify-js true client/generatedHtml.html > client/generatedHtmlMin.html
(cd client && ../../../common/htmlPacker/htmlPacker.bin generatedHtmlMin.html generatedHtml)
fi
echo "Client build success"

name="server"
warning_flags="-Wall -Wextra -Wconversion -Wshadow -Wpadded"
size_flags="-ffunction-sections -fdata-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--build-id=none"
nolibc_flags="-static -fno-pie -nostdlib -nostartfiles -ffreestanding"
debug_flags="$warning_flags $size_flags $nolibc_flags -fsanitize-undefined-trap-on-error -fsanitize=undefined -g -o $name.bin"
release_flags="$warning_flags $size_flags $nolibc_flags -Ddebug_NDEBUG -s -O2 -o release_$name.bin"
CC=${CC:-gcc}
$CC $debug_flags $name.c
$CC $release_flags $name.c
strip --remove-section .comment --remove-section .note.gnu.property release_$name.bin
echo "Server build success"