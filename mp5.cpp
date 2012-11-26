#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <vector>

#include <math.h>

#include "extX.h"
#include "ext2analysis.hpp"
#include "ext2presentation.hpp"

using namespace std;

void print_select_menu()
{
  printf("%s\r\n", "=======================================");
  printf("%s\r\n", "0. Exit");
  printf("%s\r\n", "1. Open file or device");
  printf("%s\r\n", "2. Calucation on block group #");
  printf("%s\r\n", "3. Super block information");
  printf("%s\r\n", "4. Group descriptor table");
  printf("%s\r\n", "5. Inode bitmap in group #");
  printf("%s\r\n", "6. Inode table in group #");
  printf("%s\r\n", "7. Print i-node info");
  printf("%s\r\n", "=======================================");
}

int input_num(const char* title)
{
  int num = 0;

  printf("%s >", title);
  scanf("%d", &num);

  return num;
}

void calculate_block_group()
{
  int num_of_inode = 0;
  int num_of_group =0;
  int entry =0;

  printf("%s\r\n", "***************************************");
  printf("\t %s\r\n", "Calucation on block group");
  printf("%s\r\n", "***************************************");

  printf("%-15s > ", "Input I-Node #");
  scanf("%d", &num_of_inode);
  printf("\r\n");

  num_of_group = BLOCK_GROUP_OF_INODE(num_of_inode, INODES_PER_GROUP);
  entry = ENTRY_INODE_TABLE(num_of_inode, INODES_PER_GROUP);

  printf("%s\r\n", "---------------------------------------");
  printf("%-27s : %d\r\n", "i-node num", num_of_inode);
  printf("%-27s : %d\r\n", "Block group number", num_of_group);
  printf("%-27s : %d\r\n", "The offset of i-node table", entry);
  printf("%s\r\n", "---------------------------------------");
  printf("%s", "Press any key");

  getchar();
  while(getchar() != '\n');
  printf("\r\n");
}

int open_file_from_input(char* sz_title)
{
  char filename[256];
  int fd = 0;
  fs_super_block superblk;

  printf("%s\r\n", "***************************************");
  printf("\t %s\r\n", sz_title);
  printf("%s\r\n", "***************************************");
  
  // printf("%-12s ./> ", "Filename");
  // scanf("%s", filename);
  strcpy(filename, "/dev/mapper/vg_kkd-lv_root");
  printf("%-12s ./> %s\r\n", "Filename", filename);

  fd = open(filename, O_RDONLY);
  if(fd < 0) {
    printf("Worng filename!!!\r\n");
    return -1;
  }
  printf("%s ... OK\r\n", "File opened");

  return fd;
}

void print_bitmap(const _byte* bitmap, size_t size)
{
  int col = 16;
  int line = size / col;

  printf("%4s ", "");
  for(int i = 0 ; i < col ; i++)
    printf("%02d ", i);
  printf("\r\n");

  for(int i = 0 ; i < line ; i++)
    {
      printf("%04X ", i * col);
      for(int j = 0 ; j < col; j++)
	printf("%02X ", bitmap[i * col + j]);
      printf("\r\n");
    }
}

/*
void print_inode_bitmap(int fd,
			const fs_super_block& super,
			const group_desc& gd)
{
  _byte* buf = 0;
  _dword blk_size = GET_BLOCK_SIZE(super.log_blk_size);
  _dword ret = 0;
  
  buf = new _byte[blk_size];
  
  ret = read_inode_bitmap(fd, super, gd, buf, blk_size); 
  if(ret < 0) {
    printf("error : print inode bitmap\r\n");
    return;
  }

  printf("%s\r\n", "=====================================================");
  printf("\t%s\r\n", "i-node bitmap");
  printf("%s\r\n", "-----------------------------------------------------");

  print_bitmap(buf, ret);

  delete[] buf;
  }*/

int main(int argc, char* argv[])
{
  int sel = 5;
  Ext2Analysis ext("/dev/mapper/vg_kkd-lv_root");
  Ext2Presentation present;

  present.attach_ext2(&ext);

  do
    {
      print_select_menu();
      sel = input_num("?");

      switch(sel)
	{
	case 1: 
	  //fd = open_file_from_input("Open file or device"); 
	  //load_super_block(fd, fs_super);
	  //load_group_desc(fd,fs_super, group_descs);
	  break;
	case 2: calculate_block_group(); break;
	case 3: present.print_super_block(); break;
	case 4: present.print_group_desc(); break;
	case 5: present.print_inode(input_num("inode #")); break;
	case 6: present.print_inodes_in_group(input_num("Group #")); break;

	default:  break;
	}
    }while(sel);

  return 0;
}
