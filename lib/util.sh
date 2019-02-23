#!/bin/sh

#
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Utility functions
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

base64_to_hex() {
  $CMD_ECHO -n "$1" | $CMD_BASE64 --decode | $CMD_HEXDUMP -v -e '/1 "%02x"'
}

extract_json() {
  VALUE=$(extract_json_string "$1" $2)
  $CMD_ECHO "$VALUE" | $SCRIPT_PATH/bin/JSON.sh -n
}

extract_json_string() {
  VALUELEN=${#1}
  VALUEEND=$(($VALUELEN-1))
  $CMD_ECHO -n $1 | $CMD_CUT -c $(($2 + 2))-$VALUEEND | $CMD_SED -E 's/\\(.)/\1/g'
}

extract_json_unquoted() {
  $CMD_ECHO -n $1 | $CMD_CUT -c $(($2 + 1))-
}

prepare_password() {
  HEX=`$CMD_ECHO -n $ENCEEPER_PASSWORD | $CMD_HEXDUMP -v -e '/1 "%02x"'`
  VALUELEN=${#HEX}
  MOD=$((VALUELEN % 8))
  COUNT=$((8 - MOD))
  i=1

  if [ $MOD -ne 0 ]; then
    # Pad with 0
    while [ "$i" -le "$COUNT" ]; do
      HEX=$HEX"0"
      i=$(( i + 1 ))
    done
  fi

  $CMD_ECHO -n $HEX
}
