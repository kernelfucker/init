# init
minimal init for x86_64 unix-like systems

# compile
$ clang init.c -o init -static

# rootfs, qemu etc..
$ mkdir --parents rootfs/{bin,proc}

$ cp init rootfs/

$ doas cp /bin/busybox rootfs/bin/sh

$ cd rootfs/

$ find . -print0 | cpio --null -v -o --format=newc | gzip > ../initramfs.cpio.gz

$ cd ../

$ qemu-system-x86_64 -kernel /boot/vmlinuz-linux -initrd initramfs.cpio.gz -nographic -append "console=ttyS0 init=/init"

# stat for usage
![image](https://github.com/user-attachments/assets/b9c345b8-ef13-45a4-be87-b05f8a80d88a)
