set -e

(cd client && ../../common/htmlPacker/htmlPacker.bin html/main.html generatedHtml)
echo "Client build success"