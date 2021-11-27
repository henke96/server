set -e

(cd client && ../../../common/htmlPacker/htmlPacker.bin html/main.html generatedHtml)
if test "$MINIFY" = "yes"; then
html-minifier --collapse-whitespace --remove-comments --remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes --use-short-doctype --minify-css true --minify-js true client/generatedHtml.html > client/generatedHtmlMin.html
(cd client && ../../../common/htmlPacker/htmlPacker.bin generatedHtmlMin.html generatedHtml)
fi
echo "Client build success"

../../common/build.sh server
echo "Server build success"