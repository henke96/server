set -e

(cd ../common/htmlPacker && ./build.sh)
(cd client && ../../common/htmlPacker/htmlPacker.bin html/main.html generatedHtml)
echo "Client build success"