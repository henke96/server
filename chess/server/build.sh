set -e
(cd client && ../../../common/htmlPacker/htmlPacker.bin html/main.html generatedHtml)
if test ! -z "$MINIFY"; then
html-minifier --collapse-whitespace --remove-comments --remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes --use-short-doctype --minify-css true --minify-js true client/generatedHtml.html > client/generatedHtmlMin.html
(cd client && ../../../common/htmlPacker/htmlPacker.bin generatedHtmlMin.html generatedHtml)
fi
echo "Client build success"
name="server"
common_flags="-ffunction-sections -fdata-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--build-id=none -Wall -Wextra -Wconversion -Wshadow -Wpadded"
CC=${CC:-gcc}
$CC -static -nostdlib -nostartfiles -ffreestanding -g -o $name.bin $common_flags $name.c
$CC -static -nostdlib -nostartfiles -ffreestanding -Ddebug_NDEBUG -s -O2 -o release_$name.bin $common_flags $name.c
strip --remove-section .comment --remove-section .note.gnu.property release_$name.bin
$CC -Dnolibc_YESLIBC -fsanitize=undefined -fsanitize=address -g -o debug_$name.bin $common_flags -Wno-padded $name.c # -Wpadded is broken on GCC?
echo "Server build success"