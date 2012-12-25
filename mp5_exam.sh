#! /bin/bash

# Creator : Kideok Kwon
# Date : 2012.11.05

clear
make clean
make all
# super block      
#echo ./extx if=/dev/sdb2 -s
#./extx if=/dev/sdb2 -s

# group descriptor 
#echo ./extx if=/dev/sdb2 -gt
#./extx if=/dev/sdb2 -gt

# block bitmap 
# echo ./extx if=/dev/sdb2 -bmap gno=0
# ./extx if=/dev/sdb2 -bmap gno=0

# inode bitmap
# ./extx if=/dev/mapper/vg_kkd-lv_root -imap gno=0

# inode table
# echo ./extx if=/dev/sdb2 -it gno=0
# ./extx if=/dev/sdb2 -it gno=0

# inode 
# Symbolic link 
# echo ./extx if=/dev/sdb2 -inode ino=1978 dump=false
# ./extx if=/dev/sdb2 -inode ino=1978 dump=false
# echo ./extx if=/dev/sdb2 -inode ino=1978 dump=true
# ./extx if=/dev/sdb2 -inode ino=1978 dump=true

# There exists a symbolic link in data block
# echo ./extx if=/dev/sdb2 -inode ino=1981 dump=false
# ./extx if=/dev/sdb2 -inode ino=1981 dump=false
echo ./extx if=/dev/sdb2 -inode ino=1981 dump=true
./extx if=/dev/sdb2 -inode ino=1981 dump=true

# Regular file
#echo ./extx if=/dev/sdb2 -inode ino=1980
#./extx if=/dev/sdb2 -inode ino=1980

# Directory 
#./extx if=/dev/sdb2 -inode ino=1977 dump=false
#./extx if=/dev/sdb2 -inode ino=1977 dump=true
