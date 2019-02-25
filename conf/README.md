# Initramfs configuration

This directory contains the necessary hooks, scripts and configuration files in order for Initramfs to:
- connect to the network (wired or wireless)
- fetch a key and decrypt it from the [Enceeper service](https://www.enceeper.com/)
- mount via cryptroot the root filesystem using the above decrypted key

## Recommended reading

Because of the complexity of the tools involved we insist in reading the following material:

- [Enable Wireless networks in Debian Initramfs](http://www.marcfargas.com/posts/enable-wireless-debian-initramfs/)
- Read the manual: man initramfs-tools
- [klibc C library](https://git.kernel.org/pub/scm/libs/klibc/klibc.git/), that also provides the ipconfig tool
- [Debian Initramfs repository](https://salsa.debian.org/kernel-team/initramfs-tools/)
- [Ubuntu Initramfs repository](https://git.launchpad.net/ubuntu/+source/initramfs-tools)
- [Predictable Network Interface Names](http://www.freedesktop.org/wiki/Software/systemd/PredictableNetworkInterfaceNames)

Finally a copy from https://github.com/systemd/systemd/blob/master/src/udev/udev-builtin-net_id.c

> Two character prefixes based on the type of interface:
> * en — Ethernet
> * ib — InfiniBand
> * sl — serial line IP (slip)
> * wl — wlan
> * ww — wwan

Based on the interface type (enXXX or wlXXX) we will either use the wired or wireless script to enable networking.

## Further reading and future work

- [Lock encrypted root volume on suspend in Ubuntu](https://github.com/zhongfu/ubuntu-luks-suspend)
- [Two factor authentication for harddisk encryption](https://github.com/cornelinux/yubikey-luks)
- [Use YubiKey to unlock a LUKS partition](https://github.com/agherzan/yubikey-full-disk-encryption)
- [Yubikey full disk encryption](https://github.com/tfheen/ykfde/)
- [Lock encrypted root volume on suspend in Arch Linux](https://github.com/vianney/arch-luks-suspend)
