#! /bin/bash

# Creator : Kideok Kwon
# Date : 2012.11.05

clear
make clean
make all
# super block      
# ./extx if=/dev/sdb2 -s

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
./extx if=/dev/sdb2 -inode ino=1981 dump=true

# Regular file
# ./extx if=/dev/sdb2 -inode ino=2190

# Directory 
# ./extx if=/dev/sdb2 -inode ino=1977 dump=true
