#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <vector>

#include <math.h>

#include "extX.h"
#include "ext2_analysis.hpp"

using namespace std;

int select_menu()
{
  int num = 0;

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

  printf("%s", "? > ");
  scanf("%d", &num);
  
  return num;
}

int input_group_num()
{
  int num_of_group =0;

  printf("%-12s > ", "Input group #");
  scanf("%d", &num_of_group);

  return num_of_group;
}

void calcuate_block_group()
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

void print_super_block(const fs_super_block& blk)
{
  printf("%-22s : %10s \r\n", "Last mounted on", blk.last_mounted);
  printf("%-22s : %10u \r\n", "Inode count", blk.inode_cnt);
  printf("%-22s : %10u \r\n", "Block count", blk.blk_cnt);	
  printf("%-22s : %10u \r\n", "Res block count", blk.res_blk_cnt);	
  printf("%-22s : %10u \r\n", "Free block count", blk.free_blk_cnt);
  printf("%-22s : %10u \r\n", "Free inode count", blk.free_inode_cnt);
  printf("%-22s : %10u \r\n", "First data node block", blk.first_data_blk);
  printf("%-22s : %10u \r\n", "Block size", GET_BLOCK_SIZE(blk.log_blk_size));	
  printf("%-22s : %10u \r\n", "Fragment size", (_dword)(_1K_BLOCK * pow(2,blk.log_frag_size)));
  printf("%-22s : %10u \r\n", "Blocks per group", blk.blks_per_group);	
  printf("%-22s : %10u \r\n", "Fragments per group", blk.frags_per_group);	
  printf("%-22s : %10u \r\n", "Inode per group", blk.inode_per_group);
  printf("%-22s : %10u \r\n", "Mount count", blk.mount_cnt);	
  printf("%-22s : %10u \r\n", "Max mount count", (_dword)blk.max_mount_cnt);	
  printf("%-22s : %6s%4X \r\n", "Magic", "0x", blk.magic);
  printf("%-22s : %10u \r\n", "Block group #", (_dword)blk.blk_group_num);	
  printf("%-22s : %10u \r\n", "Inode size", blk.inode_size);	
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

inline void close_file_opened(int fd)
{
  close(fd);
}

/*
void load_super_block(int fd, fs_super_block& super)
{
  if(fd < 0) return;

  memset(&super, 0, sizeof(fs_super_block));
  if(read_super_block(fd, &super) < 0)
    printf("%s ... Failed\r\n", "Load superblock");
  else
    printf("%s ... Ok\r\n", "Load superblock");
    }*/

// Print group descriptors
void print_title_of_group_desc() 
{
  // Print titile
  printf("%s\r\n", "======================================================================");
  printf("%3s  %10s  %10s  %10s  %8s  %8s  %8s\r\n",
	 "no", "blk map", "ino map", "ino table",
	 "free blk", "free ino", "used dir");
  printf("%s\r\n", "----------------------------------------------------------------------");
}

void print_element_of_group_desc(const vector<group_desc>& groups)
{
  // Print group descriptor
  size_t size = groups.size();

  for(size_t i = 0 ; i < size ; i++) {
    printf("%3d  %9u   %9u   %9u   %7u   %7u   %7u \r\n",
	   i, 
	   groups[i].blk_bitmap, 
	   groups[i].inode_bitmap, 
	   groups[i].inode_table,
	   groups[i].free_blk_cnt, 
	   groups[i].free_inode_cnt, 
	   groups[i].useddir_cnt);
  }
}

void print_element_of_inode(const vector<i_node>& inodes)
{
  // Print group descriptor
  size_t size = inodes.size();

  for(size_t i = 0 ; i < size ; i++) {
    printf("%3u  %04X   %04X   %10u   %04X   %10u   %10u \r\n",
	   i, 
	   inodes[i].mode, 
	   inodes[i].uid, 
	   inodes[i].size,
	   inodes[i].gid, 
	   inodes[i].link_cnt, 
	   inodes[i].blocks);
  }
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


void print_group_desc(const vector<group_desc>& groups)
{
  print_title_of_group_desc();
  print_element_of_group_desc(groups);
}

/*
void load_group_desc(int fd, 
		     const fs_super_block& fs_super, 
		     vector<group_desc>& groups)
{
  int cnt = 0;

  // Block size = 1K * 2 ^ log_blk_size
  _dword blk_size = _1K_BLOCK * pow(2, fs_super.log_blk_size);

  cnt = read_group_desc(fd, blk_size, fs_super, groups);
  printf("%s : %d\r\n", "Totoal group descriptors", groups.size());

  }*/

int main(int argc, char* argv[])
{
  int sel = 5;
  ExtAnalysis ext("/dev/mapper/vg_kkd-lv_root");

  do
    {
      sel = select_menu();
      switch(sel)
	{
	case 1: 
	  //fd = open_file_from_input("Open file or device"); 
	  //load_super_block(fd, fs_super);
	  //load_group_desc(fd,fs_super, group_descs);
	  break;
	case 2: calcuate_block_group(); break;
	case 3: 
	  print_super_block(ext.get_super_block());
	  break;
	case 4: 
	  print_group_desc(ext.get_group_desc()); 
	  break;
	case 5:
	  {
	    // int num = input_group_num();
	    // group_desc gd = group_descs[num];
	    // print_inode_bitmap(fd, fs_super, gd);	    
	    // break;
	  }
	case 6:
	  {
	    vector<ExtAnalysis::inode_info> inodes;
	    int num = input_group_num();

	    ext.get_inodes(num, inodes);
	    //	    printf("%d\r\n", inodes.size());
	    // print_element_of_inode(inodes);
	  }

	default:  break;
	}
    }while(sel);

  return 0;
}
