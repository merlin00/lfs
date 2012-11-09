// Data structures for the DOS partition table
/*
  In the first sector
  0-446 Boot
  446-461 Partition Table Entry 1
  462-477 Partition Table Entry 2
  478-493 Partition Table Entry 3
  494-509 Partition Table Entry 4
  510-511 Signature value(0xAA55)
 */
typedef unsigned char _byte;
typedef unsigned int _int32;

#define DOS_SIGNATURE 0xAA55
#define BOOTCODE_SIZE 446

typedef struct _partition_table {
  _byte bootable;       //Bootable Flag
  _int32 start_of_chs : 24; //Starting CHS Address
  _byte type;
  _int32 end_of_chs : 24;
  _int32 start_of_lba : 24;
  _int32 size;
}partition_table;

