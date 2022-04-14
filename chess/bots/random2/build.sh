set -e
(cd ../common/gen && ./gen.bin)

../../../common/hc/build.sh .
echo "Build success"