gcc -static init.c -o rootfs/init

rm -rf rootfs/dev/.keep
rm -rf rootfs/mnt/.keep
rm -rf rootfs/proc/.keep
rm -rf rootfs/root/.keep
rm -rf rootfs/sys/.keep
rm -rf rootfs/tmp/.keep
rm -rf rootfs/var/cache/fontconfig/.keep

cd programs
for i in *.c
do
    echo "CC $i"
    gcc -static "$i" -o "../rootfs/bin/${i%.c}"
done
cd ..

chmod +x rootfs/init
cd rootfs
find . | cpio -R root:root -H newc -o | xz -9 --check=none > ../rootfs.cpio.xz
cd ..
cp rootfs.cpio.xz iso/boot/rootfs.xz
rm rootfs.cpio.xz
cd iso
xorriso -as mkisofs -isohybrid-mbr ../isohdpfx.bin -c boot/syslinux/boot.cat -b boot/syslinux/isolinux.bin -no-emul-boot -boot-load-size 4 -boot-info-table -o ../ingolna.iso ../iso/
