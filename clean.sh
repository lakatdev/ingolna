rm -rf rootfs/bin/*
rm -rf iso/boot/rootfs.xz
rm -rf ingolna.iso
rm -rf rootfs/init

touch rootfs/bin/.keep
touch rootfs/dev/.keep
touch rootfs/lib/.keep
touch rootfs/mnt/.keep
touch rootfs/proc/.keep
touch rootfs/root/.keep
touch rootfs/sys/.keep
touch rootfs/tmp/.keep
