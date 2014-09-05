#!/usr/bin/env bash
# import EXPECTED RESPONSES VARS
. ./responses.sh


# force unpair
latch -u > /dev/null 2>&1

echo 'Test latch conf: without read permissions'
if latch -f /etc/latch/latch.conf 2>&1 | grep "$TEST_CONFIG_ACCESS_ERROR__RES" > /dev/null 2>&1
then
  echo -e '\e[32mIt was great! =)\e[0m'
else
  echo -e '\e[31mUpps.. Something was wrong :(\e[0m'
fi
