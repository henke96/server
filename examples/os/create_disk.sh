#!/bin/sh
set -e
script_dir="$(dirname $0)"
cleanup() {
    set +e
    umount "$script_dir/mnt"
    rmdir "$script_dir/mnt"
    losetup -d $dev
}

dd if=/dev/zero of="$script_dir/disk.img" bs=1024 count=35000
chmod a+rw "$script_dir/disk.img"

dev=`losetup --show -f "$script_dir/disk.img"`
trap cleanup EXIT

parted -s $dev \
mklabel gpt \
mkpart EFI 1Mib 34Mib set 1 esp on

mkfs -t fat -F 32 ${dev}p1

mkdir -p "$script_dir/mnt"
mount ${dev}p1 "$script_dir/mnt"
mkdir -p "$script_dir/mnt/EFI/BOOT"
cp "$script_dir/bootloader.efi" "$script_dir/mnt/EFI/BOOT/bootx64.efi"
