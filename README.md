# enceeper-boot

A secure solution for unattended booting a Debian/Ubuntu (or any other GNU/Linux) distro that uses full disk encryption via Initramfs utilizing Enceeper.

> Warning: Before starting we recommend to have a working full disk encrypted system that is unlocked via a passphrase to avoid misconfigurations and locking yourself out of your system. Also taking a full backup of your system or testing everything in a VM are also good approaches!

## Introduction

The Enceeper app (https://github.com/enceeper/enceeper) and the Enceeper service (https://www.enceeper.com/) can be used to securely store and retrieve credentials (usernames, passwords, API keys etc). We wanted to provide a solution that will allow a GNU/Linux distro to securely boot utilizing Enceeper. To this end we created the following:

* An AES256-CCM decryptor (utilizing a stripped-down version of [Nettle](https://git.lysator.liu.se/nettle/nettle) or via the OpenSSL library)
* Scripts and configs to allow Initramfs to connect to the Internet (either via wired or wireless interface)
* Scripts and configs to connect to the Enceeper service and retrieve a single key for unlocking the LUKS encrypted partition(s)

## Prerequisites

Before continuing make sure that you have installed the latest software:

```bash
sudo apt update
sudo apt dist-upgrade
```

and once completed you have rebooted the system:

```bash
sudo reboot
```

## Installation with Makefile

```bash
git clone https://github.com/enceeper/enceeper-boot
cd enceeper-boot
sudo make install
```

Now edit the file */etc/enceeper.conf* and provide the slot ID and the decryption password.

Then add the Enceeper password to an additional slot of your encrypted disk (where */dev/sda5* is the name of your disk):

```bash
sudo ./enceeper.sh > /dev/shm/enceeper_key
sudo cryptsetup luksAddKey /dev/sda5 /dev/shm/enceeper_key
sudo shred -z -u /dev/shm/enceeper_key
```

### Configure the network

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

Where enp0s3 is the name of your network interface and panic is the number of seconds to wait before rebooting the system in case something goes wrong.

### Apply everything

Backup your current Initramfs and update:

```bash
sudo cp /boot/initrd.img-`uname -r` /boot/initrd.img-`uname -r`.back
sudo update-initramfs -k `uname -r` -u
sudo update-grub
```

You can now enjoy your unattended boot 😉.
