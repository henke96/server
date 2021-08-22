set -e

(cd ../../htmlPacker && ./build.sh)
(cd client && ../../../htmlPacker/htmlPacker html/main.html generatedHtml)
echo "Client build success"

common_flags="-Wpedantic -Wall -Wextra -Wconversion -Wno-unused-parameter"
gcc -fsanitize=undefined -fsanitize=address -g -DUNREACHABLE='assert(0)' -o server $common_flags server.c
gcc -static -s -O2 -DNDEBUG -DUNREACHABLE='__builtin_unreachable()' -o release_server $common_flags server.c
echo "Server build success"