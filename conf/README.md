# Initramfs configuration

This directory contains the necessary hooks, scripts and configuration files in order for Initramfs to:
- connect to the network (wired or wireless)
- fetch a key and decrypt it from the [Enceeper service](https://www.enceeper.com/)
- mount via cryptroot the root filesystem using the above decrypted key

> Warning: Before starting we recommend to have a working encrypted system that is unlocked via a passphrase to avoid misconfigurations and locking yourself out of your system.

## Required reading

Because of the complexity of the involved tools we insist in reading the following materials:

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

## Copy files (tested on Debian/Ubuntu)

```bash
sudo install -o root -g root -m 0400 etc/initramfs-tools/wpa_supplicant.conf /etc/initramfs-tools/wpa_supplicant.conf

sudo install -o root -g root etc/initramfs-tools/hooks/enceeper /etc/initramfs-tools/hooks/enceeper

sudo install -o root -g root etc/initramfs-tools/scripts/enceeper_util /etc/initramfs-tools/scripts/enceeper_util
sudo install -o root -g root etc/initramfs-tools/scripts/init-premount/wired_networking /etc/initramfs-tools/scripts/init-premount/wired_networking
sudo install -o root -g root etc/initramfs-tools/scripts/init-premount/wireless_networking /etc/initramfs-tools/scripts/init-premount/wireless_networking
sudo install -o root -g root etc/initramfs-tools/scripts/local-bottom/kill_wireless /etc/initramfs-tools/scripts/local-bottom/kill_wireless
sudo install -o root -g root etc/initramfs-tools/scripts/local-bottom/shred_cache /etc/initramfs-tools/scripts/local-bottom/shred_cache
```

## Edit configuration files

In case you are utilizing a wireless interface for networking you will need to edit the following files:

- /etc/initramfs-tools/wpa_supplicant.conf
- /etc/initramfs-tools/hooks/enceeper

In the wpa_supplicant.conf you will have to provide the correct wireless name (ssid) and the WPA/WPA2 key (psk: Pre Shared Key). In the enceeper script you will need to uncomment the lines for utilizing the wireless interface and update the line: *manual_add_modules ath9k_htc* and provide the modules required by your wireless interface. To find out the required modules execute:

```bash
ethtool -i wlXXX | grep driver
```

Regardless of wired or wireless connection you will need to edit the file /etc/default/grub and add/edit the following line:

```
GRUB_CMDLINE_LINUX="ip=:::::enp0s3:on panic=10"
```

Where enp0s3 is the name of your interface and panic is the number of seconds to wait before rebooting the system in case of panic.

Backup your current Initramfs and update:

```bash
sudo cp /boot/initrd.img-`uname -r` /boot/initrd.img-`uname -r`.back
sudo update-initramfs -k `uname -r` -u
sudo update-grub
```

## Further reading and future work

- [Lock encrypted root volume on suspend in Ubuntu](https://github.com/zhongfu/ubuntu-luks-suspend)
- [Two factor authentication for harddisk encryption](https://github.com/cornelinux/yubikey-luks)
- [Use YubiKey to unlock a LUKS partition](https://github.com/agherzan/yubikey-full-disk-encryption)
- [Yubikey full disk encryption](https://github.com/tfheen/ykfde/)
- [Lock encrypted root volume on suspend in Arch Linux](https://github.com/vianney/arch-luks-suspend)
