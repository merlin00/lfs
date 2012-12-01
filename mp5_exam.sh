#! /bin/bash

# Creator : Kideok Kwon
# Date : 2012.11.05

clear
make clean
make all
# ./extx if=/dev/mapper/vg_kkd-lv_root -s
# ./extx if=/dev/mapper/vg_kkd-lv_root -gt
# ./extx if=/dev/mapper/vg_kkd-lv_root -bmap gno=37
# ./extx if=/dev/mapper/vg_kkd-lv_root -imap gno=0
# ./extx if=/dev/mapper/vg_kkd-lv_root -it gno=0
./extx if=/dev/mapper/vg_kkd-lv_root -inode ino=2
