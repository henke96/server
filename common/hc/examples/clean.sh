# Make sure it's not run from the wrong folder.
if test "$(dirname $0)" != "."; then
    echo "Usage: ./clean.sh"
    exit 1
fi

# Clean up all files covered by .gitignore.
find . -name "*.bin*" -type f -delete
find . -name "*.efi*" -type f -delete
find . -name "*.exe*" -type f -delete
find . -name "*.wasm*" -type f -delete
find . -name "*.img" -type f -delete
