#!/bin/sh

#
# SPDX-License-Identifier: GPL-3.0-or-later
#
# The enceeper script to extract a key from a slot
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

# Get the script base
SCRIPT_PATH=$(dirname $0)

# Load config
ENCEEPER_CONFING=`grep = /etc/enceeper.conf | sed 's/ *= */=/g'`
eval "$ENCEEPER_CONFING"

if [ -f "/tmp/enceeper_slot_$ENCEEPER_SLOT_ID" ]; then
  cat /tmp/enceeper_slot_$ENCEEPER_SLOT_ID
  exit 0
fi

# Include files
. $SCRIPT_PATH/lib/params.sh
. $SCRIPT_PATH/lib/util.sh
. $SCRIPT_PATH/lib/network.sh

# Report error when using empty SLOT_ID or PASSWORD
if [ ${#ENCEEPER_SLOT_ID} -eq 0 ]; then
  $CMD_ECHO "ERROR: The ENCEEPER_SLOT_ID cannot be empty"
  exit 10
fi

if [ ${#ENCEEPER_PASSWORD} -eq 0 ]; then
  $CMD_ECHO "ERROR: The ENCEEPER_PASSWORD cannot be empty"
  exit 11
fi

# Fetch the slot
get_slot
RET_VAL=$?

if [ $RET_VAL -ne 0 ]; then
  $CMD_ECHO "ERROR: Could not fetch the slot with return value: $RET_VAL"
  exit 1
fi

# Extract JSON data
JSON=`$SCRIPT_PATH/bin/JSON.sh < $TMP_FILENAME | $CMD_GREP '\["result","[^"]*"\]'`

SLOT=`$CMD_ECHO "$JSON" | $CMD_GREP '\["result","slot"\]'`
VALUE=`$CMD_ECHO "$JSON" | $CMD_GREP '\["result","value"\]'`

JSON_SLOT=$(extract_json "$SLOT" 18)
JSON_VALUE=$(extract_json "$VALUE" 19)

# Check versions (v=1)
SLOT_V=$(extract_json_unquoted "$($CMD_ECHO "$JSON_SLOT" | $CMD_GREP '\["v"\]')" 6)
VALUE_V=$(extract_json_unquoted "$($CMD_ECHO "$JSON_VALUE" | $CMD_GREP '\["v"\]')" 6)

if [ $SLOT_V -ne 1 ]; then
  $CMD_ECHO "ERROR: Cannot handle slot version: $SLOT_V"
  exit 20
fi
if [ $VALUE_V -ne 1 ]; then
  $CMD_ECHO "ERROR: Cannot handle value version: $VALUE_V"
  exit 21
fi

# Calc slot decryption key
SCRYPT_SALT=$(extract_json_string "$($CMD_ECHO "$JSON_SLOT" | $CMD_GREP '\["scrypt"\]')" 11)
SCRYPT_KEY=`$SCRIPT_PATH/bin/scrypt $(base64_to_hex $SCRYPT_SALT) $(prepare_password) $SCRYPT_N`
RET_VAL=$?

if [ $RET_VAL -ne 0 ]; then
  $CMD_ECHO "ERROR: Scrypt failed with return value: $RET_VAL"
  exit 2
fi

# Decrypt the slot
SLOT_IV=$(extract_json_string "$($CMD_ECHO "$JSON_SLOT" | $CMD_GREP '\["iv"\]')" 7)
SLOT_CIPHER=$(extract_json_string "$($CMD_ECHO "$JSON_SLOT" | $CMD_GREP '\["ct"\]')" 7)
SCRYPT_LEN=${#SCRYPT_KEY}
SCRYPT_LEN=$((SCRYPT_LEN / 2))
SLOT_PLAIN=`$SCRIPT_PATH/bin/decryptor $(base64_to_hex $SLOT_IV) $($CMD_ECHO $SCRYPT_KEY | $CMD_CUT -c 1-$SCRYPT_LEN) $(base64_to_hex $SLOT_CIPHER)`
RET_VAL=$?
SLOT_KEY=`$CMD_ECHO -n "$SLOT_PLAIN" | $CMD_XXD -r -p`

if [ $RET_VAL -ne 0 ]; then
  $CMD_ECHO "ERROR: Failed to decrypt slot with return value: $RET_VAL"
  exit 3
fi

# Decrypt the value and get the password details
KEY_IV=$(extract_json_string "$($CMD_ECHO "$JSON_VALUE" | $CMD_GREP '\["iv"\]')" 7)
KEY_CIPHER=$(extract_json_string "$($CMD_ECHO "$JSON_VALUE" | $CMD_GREP '\["ct"\]')" 7)
KEY_PLAIN=`$SCRIPT_PATH/bin/decryptor $(base64_to_hex $KEY_IV) $SLOT_KEY $(base64_to_hex $KEY_CIPHER)`
RET_VAL=$?

if [ $RET_VAL -ne 0 ]; then
  $CMD_ECHO "ERROR: Failed to decrypt value with return value: $RET_VAL"
  exit 3
fi

# Return the password used in LUKS
JSON_PASS=`$CMD_ECHO "$KEY_PLAIN" | $CMD_XXD -r -p | $SCRIPT_PATH/bin/JSON.sh`
PASS_VER=$(extract_json_unquoted "$($CMD_ECHO "$JSON_PASS" | $CMD_GREP '\["v"\]')" 6)
PASS_VAL=$(extract_json_string "$($CMD_ECHO "$JSON_PASS" | $CMD_GREP '\["p",0,"v"\]')" 12)

if [ $PASS_VER -ne 1 ]; then
  $CMD_ECHO "ERROR: Cannot handle version: $PASS_VER"
  exit 4
fi

# We are inside Initramfs, cache password
if [ -f "/bin/wgetf" ]; then
  $CMD_ECHO -n "$PASS_VAL" > /tmp/enceeper_slot_$ENCEEPER_SLOT_ID
fi

$CMD_ECHO -n "$PASS_VAL"
