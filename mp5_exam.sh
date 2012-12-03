#! /bin/bash

# Creator : Kideok Kwon
# Date : 2012.11.05

clear
make clean
make all
# super block      
# ./extx if=/dev/mapper/vg_kkd-lv_root -s

# group descriptor 
# ./extx if=/dev/mapper/vg_kkd-lv_root -gt

# block bitmap 
# ./extx if=/dev/mapper/vg_kkd-lv_root -bmap gno=37

# inode bitmap
# ./extx if=/dev/mapper/vg_kkd-lv_root -imap gno=0

# inode table
# ./extx if=/dev/mapper/vg_kkd-lv_root -it gno=0

# inode 
# Symbolic link 
# ./extx if=/dev/mapper/vg_kkd-lv_root -inode ino=1719866 symbolic link example

# Regular file
# ./extx if=/dev/mapper/vg_kkd-lv_root -inode ino=1719866

# Directory 
./extx if=/dev/mapper/vg_kkd-lv_root -inode ino=1714243
