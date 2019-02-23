#!/bin/sh

#
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Test the script functionality
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

# Include files
. $SCRIPT_PATH/lib/params.sh
. $SCRIPT_PATH/lib/util.sh
. $SCRIPT_PATH/lib/network.sh

TOTAL_TESTS=8
CURRENT_TEST=1

ENCEEPER_SLOT_ID="a863efaace9fccdac03f8f79a85f229e"
ENCEEPER_PASSWORD="ThisLittlePiggyWasHappyWithEncryption"
SAMPLE_JSON_STRING='{"v":1,"outcome":"OK","inner":"{\"key\":\"value\"}"}'

# Input params:
# $1 the name of the test
# $2 the output of the test
# $3 the expected output
log_test_outcome() {
  $CMD_ECHO -n "Testing $1 ($CURRENT_TEST/$TOTAL_TESTS): "
  if [ "$2" = "$3" ]; then
    $CMD_ECHO "OK"
  else
    $CMD_ECHO "FAILED [$2]"
  fi
  CURRENT_TEST=$(( CURRENT_TEST + 1 ))
}

# Test good URL
network_fetch "https://www.enceeper.com/api/v1/"
log_test_outcome "network_fetch_ok" $? 0

# Test bad URL
network_fetch "https://www.enceeper.com/api/v1/bad"
log_test_outcome "network_fetch_error" $? 2

# Test base64 to hex
log_test_outcome "base64_to_hex" `base64_to_hex "LbxMSw=="` "2dbc4c4b"

# Test password padding
log_test_outcome "prepare_password" `prepare_password` "546869734c6974746c655069676779576173486170707957697468456e6372797074696f6e000000"

SAMPLE_JSON=`$CMD_ECHO $SAMPLE_JSON_STRING | $SCRIPT_PATH/bin/JSON.sh`

# Test value extract1
VER=$(extract_json_unquoted "$($CMD_ECHO "$SAMPLE_JSON" | $CMD_GREP '\["v"\]')" 6)
log_test_outcome "extract_json_unquoted" $VER 1

# Test value extract2
OUT=$(extract_json_string "$($CMD_ECHO "$SAMPLE_JSON" | $CMD_GREP '\["outcome"\]')" 12)
log_test_outcome "extract_json_string" $OUT "OK"

# Test value extract3
INNER=$(extract_json "$($CMD_ECHO "$SAMPLE_JSON" | $CMD_GREP '\["inner"\]')" 10)
log_test_outcome "extract_json" "$INNER" "[\"key\"]	\"value\""

# Test slot fetch
get_slot
log_test_outcome "get_slot" $? 0
