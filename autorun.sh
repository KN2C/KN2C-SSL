#!/bin/bash

pkill -f "kn2cssl"
read -rsp $'Press any key to continue...\n' -n1 key

cd bin

while true; do

  pkill -f "kn2cssl"
  ./kn2cssl nogui &

  OLD_MD5=$(md5sum kn2cssl)
  NEW_MD5=$OLD_MD5
  while [ "$OLD_MD5" == "$NEW_MD5" ]; do
    echo `date`
    sleep 15
    NEW_MD5=$(md5sum kn2cssl)
  done

done
