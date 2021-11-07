set -e
(cd client && ../../../common/htmlPacker/htmlPacker.bin html/main.html generatedHtml)
if test "$MINIFY" = "yes"; then
html-minifier --collapse-whitespace --remove-comments --remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes --use-short-doctype --minify-css true --minify-js true client/generatedHtml.html > client/generatedHtmlMin.html
(cd client && ../../../common/htmlPacker/htmlPacker.bin generatedHtmlMin.html generatedHtml)
fi
echo "Client build success"
name="server"
common_flags="-static -Wall -Wextra -Wconversion -Wshadow -Wpadded -fno-pie -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--build-id=none"
CC=${CC:-gcc}
$CC -nostdlib -nostartfiles -ffreestanding -g -o $name.bin $common_flags $name.c
$CC -nostdlib -nostartfiles -ffreestanding -Ddebug_NDEBUG -s -O2 -o release_$name.bin $common_flags $name.c
strip --remove-section .comment --remove-section .note.gnu.property release_$name.bin
$CC -Dnolibc_YESLIBC -fsanitize=undefined -g -o debug_$name.bin $common_flags $name.c
echo "Server build success"