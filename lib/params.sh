#!/bin/sh

#
# SPDX-License-Identifier: GPL-3.0-or-later
#
# The params used in the scripts
#
# Copyright (C) 2019 Vassilis Poursalidis (poursal@gmail.com)
#
# This program is free software: you can redistribute it and/or modify it under the terms of the
# GNU General Public License as published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program. If
# not, see <https://www.gnu.org/licenses/>.
#

# Commands
CMD_ECHO=/bin/echo
CMD_CAT=cat
CMD_TR=tr
CMD_HEAD=head
CMD_GREP=grep
CMD_XARGS=xargs
CMD_BASE64=base64
CMD_HEXDUMP=hexdump
CMD_CUT=cut
CMD_SED=sed
CMD_SLEEP=sleep
CMD_DATE=date
CMD_XXD=xxd

# Random filename
RANDOM_VALUE=`$CMD_CAT /dev/urandom | $CMD_TR -cd 'a-f0-9' | $CMD_HEAD -c 16`
TMP_FILENAME="/tmp/enceeper_"$RANDOM_VALUE

# Enceeper service
ENCEEPER_BASE_URL=https://www.enceeper.com/api/v1/user/slots
NETWORK_TIMEOUT=10
MAX_RETRIES=10

# Scrypt N
SCRYPT_N=32768
