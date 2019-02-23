#!/bin/sh

#
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Network functions
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

# Check if we need to wait for approval
get_slot() {
  # Try to get the slot
  URL="$ENCEEPER_BASE_URL/$ENCEEPER_SLOT_ID"
  network_fetch $URL

  if [ $? -eq 0 ]; then
    JSON=`$SCRIPT_PATH/bin/JSON.sh < $TMP_FILENAME | $CMD_GREP '\["result","[^"]*"\]'`
    LINE=`$CMD_ECHO "$JSON" | $CMD_GREP '\["result","ref"\]'`
    REF=$(extract_json_unquoted "$LINE" 17)

    if [ "$REF" = "null" ]; then
      # We got the slot
      return 0
    else
      REFLEN=${#REF}
      REFVAL=`$CMD_ECHO "$REF" | $CMD_CUT -c 2-$((REFLEN - 1))`
      URL2="$ENCEEPER_BASE_URL/check/$REFVAL"

      LINE=`$CMD_ECHO "$JSON" | $CMD_GREP '\["result","ttl"\]'`
      NOW=`$CMD_DATE +%s`
      TTL=`$CMD_ECHO -n $LINE | $CMD_CUT -c 18-`
      EXP=$((NOW + TTL))
      SLEEP=$((TTL / MAX_RETRIES))

      while true; do
        # Sleep between retries
        $CMD_SLEEP $SLEEP
        # Check if we got approved
        network_fetch $URL2

        if [ $? -eq 0 ]; then
          # We are done, we got the slot
          return 0
        fi

        # We will stop retrying after expiration
        NOW=`$CMD_DATE +%s`
        if [ $EXP -lt $NOW ]; then
          return 2
        fi
      done
    fi
  else
    return 1
  fi
}

# Fetch a single URL
network_fetch() {
  if [ -f "/bin/wgetf" ]; then
    # When we run this script inside initramfs
    WGET="/bin/wgetf --timeout $NETWORK_TIMEOUT --ca-certificate=/etc/ca-certificates.crt -S $1 -O$TMP_FILENAME"
  else
    WGET="wget --timeout $NETWORK_TIMEOUT -S $1 -O$TMP_FILENAME"
  fi

  ret=$($WGET 2>&1 | $CMD_GREP "HTTP/" | $CMD_XARGS)

  if [ "$ret" = "HTTP/1.1 200 OK" ]; then
    return 0
  elif [ "$ret" = "HTTP/1.1 428 Precondition Required" ]; then
    return 1
  else
    return 2
  fi
}
