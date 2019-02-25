.PHONY: all install

INSTALL_DIR := /usr/local/enceeper

all:

install:
	# Enceeper config file, must be edited
	install -o root -g root -m 0600 enceeper.conf.example "$(DESTDIR)/etc/enceeper.conf"
	# Main script
	install -D -o root -g root enceeper.sh                "$(DESTDIR)$(INSTALL_DIR)/enceeper.sh"
	# Helper binaries
	install -D -o root -g root bin/decryptor              "$(DESTDIR)$(INSTALL_DIR)/bin/decryptor"
	install -D -o root -g root bin/JSON.sh                "$(DESTDIR)$(INSTALL_DIR)/bin/JSON.sh"
	install -D -o root -g root bin/scrypt                 "$(DESTDIR)$(INSTALL_DIR)/bin/scrypt"
	# Script utils
	install -D -o root -g root lib/network.sh             "$(DESTDIR)$(INSTALL_DIR)/lib/network.sh"
	install -D -o root -g root lib/params.sh              "$(DESTDIR)$(INSTALL_DIR)/lib/params.sh"
	install -D -o root -g root lib/util.sh                "$(DESTDIR)$(INSTALL_DIR)/lib/util.sh"

	# WPA config, must be edited (if wireless network is required during boot)
	install -o root -g root -m 0600 conf/etc/initramfs-tools/wpa_supplicant.conf               "$(DESTDIR)/etc/initramfs-tools/wpa_supplicant.conf"

	# The hook script to glue enceeper with Initramfs
	install -o root -g root conf/etc/initramfs-tools/hooks/enceeper                            "$(DESTDIR)/etc/initramfs-tools/hooks/enceeper"

	# Script utils inside Initramfs
	install -o root -g root conf/etc/initramfs-tools/scripts/enceeper_util                     "$(DESTDIR)/etc/initramfs-tools/scripts/enceeper_util"
	install -o root -g root conf/etc/initramfs-tools/scripts/init-premount/wired_networking    "$(DESTDIR)/etc/initramfs-tools/scripts/init-premount/wired_networking"
	install -o root -g root conf/etc/initramfs-tools/scripts/init-premount/wireless_networking "$(DESTDIR)/etc/initramfs-tools/scripts/init-premount/wireless_networking"
	install -o root -g root conf/etc/initramfs-tools/scripts/local-bottom/kill_wireless        "$(DESTDIR)/etc/initramfs-tools/scripts/local-bottom/kill_wireless"
	install -o root -g root conf/etc/initramfs-tools/scripts/local-bottom/shred_cache          "$(DESTDIR)/etc/initramfs-tools/scripts/local-bottom/shred_cache"

uninstall:
	rm -r "$(DESTDIR)$(INSTALL_DIR)"
	rm "$(DESTDIR)/etc/initramfs-tools/hooks/enceeper"
	rm "$(DESTDIR)/etc/initramfs-tools/scripts/enceeper_util"
	rm "$(DESTDIR)/etc/initramfs-tools/scripts/init-premount/wired_networking"
	rm "$(DESTDIR)/etc/initramfs-tools/scripts/init-premount/wireless_networking"
	rm "$(DESTDIR)/etc/initramfs-tools/scripts/local-bottom/kill_wireless"
	rm "$(DESTDIR)/etc/initramfs-tools/scripts/local-bottom/shred_cache"
