# init
minimal init for unix-like systems

# compile
$ clang init.c -o init -static

# rootfs, qemu etc..
$ mkdir --parents rootfs/{bin,proc}

$ doas cp /bin/sh rootfs/bin/

$ cd rootfs/

$ find . -print0 | cpio --null -v -o --format=newc | gzip > ../initramfs.cpio.gz

$ qemu-system-x86_64 -kernel /boot/vmlinuz-linux -initrd initramfs.cpio.gz -nographic -append "console=ttyS0 init=/init"

# stat
![image](https://github.com/user-attachments/assets/b9c345b8-ef13-45a4-be87-b05f8a80d88a)
