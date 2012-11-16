#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "extX.h"

using namespace std;

int select_menu()
{
  int num = 0;

  printf("%s\r\n", "=======================================");
  printf("%s\r\n", "0. Exit");
  printf("%s\r\n", "1. Calucation on block group #");
  printf("%s\r\n", "2. Print super block");
  printf("%s\r\n", "3. Print group descriptor info");
  printf("%s\r\n", "4. Print i-node info");
  printf("%s\r\n", "=======================================");

  printf("%s", "? > ");
  scanf("%d", &num);
  printf("\r\n");
  
  return num;
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

void print_superblock(fs_superblock* blk)
{
  printf("%-22s : %10s \r\n", "Last mounted on", blk->last_mounted);
  printf("%-22s : %10u \r\n", "Inode count", blk->inode_cnt);
  printf("%-22s : %10u \r\n", "Block count", blk->blk_cnt);	
  printf("%-22s : %10u \r\n", "Res block count", blk->res_blk_cnt);	
  printf("%-22s : %10u \r\n", "Free block count", blk->free_blk_cnt);
  printf("%-22s : %10u \r\n", "Free inode count", blk->free_inode_cnt);
  printf("%-22s : %10u \r\n", "First data node block", blk->first_data_blk);
  printf("%-22s : %10u \r\n", "Log block size", blk->log_blk_size);	
  printf("%-22s : %10u \r\n", "Log frag size", blk->log_frag_size);
  printf("%-22s : %10u \r\n", "Block per group", blk->blk_per_grp);	
  printf("%-22s : %10u \r\n", "Frag per group", blk->frag_per_grp);	
  printf("%-22s : %10u \r\n", "Inode per group", blk->inode_per_grp);
  printf("%-22s : %10u \r\n", "Mount count", blk->mount_cnt);	
  printf("%-22s : %10d \r\n", "Max mount count", (short)blk->max_mount_cnt);	
  printf("%-22s : %6s%4X \r\n", "Magic", "0x", blk->magic);
  printf("%-22s : %10d \r\n", "Block group #", (short)blk->blk_grp_num);	
  printf("\r\n");
}

int open_file_from_input(char* sz_title)
{
  char filename[256];
  int fd = 0;
  fs_superblock superblk;

  printf("%s\r\n", "***************************************");
  printf("\t %s\r\n", sz_title);
  printf("%s\r\n", "***************************************");
  
  printf("%-12s ./> ", "Filename");
  scanf("%s", filename);

  fd = open(filename, O_RDONLY);
  if(fd < 0) {
    printf("Worng filename!!!\r\n");
    return -1;
  }
  printf("%s ... OK\r\n\r\n", "File opened");

  return fd;
}

inline void close_file_opened(int fd)
{
  close(fd);
}

void superblock()
{
  fs_superblock super;
  int fd = open_file_from_input("Superblock");

  if(fd < 0) return;

  memset(&super, 0, sizeof(fs_superblock));
  get_superblock_from_fs(fd, &super);

  print_superblock(&super);

  close_file_opened(fd);  
}

void print_grp_des(vector<grp_des> grps)
{
  size_t size = grps.size();

  printf("%s\r\n", "======================================================================");
  printf("%3s  %10s  %10s  %10s  %8s  %8s  %8s\r\n",
	 "no", "blk map", "ino map", "ino table",
	 "free blk", "free ino", "used dir");
  printf("%s\r\n", "----------------------------------------------------------------------");

  for(size_t i = 0 ; i < size ; i++) {
    printf("%3d  %9u   %9u   %9u   %7u   %7u   %7u \r\n",
	   i, 
	   grps[i].blk_bitmap, 
	   grps[i].inode_bitmap, 
	   grps[i].inode_table,
	   grps[i].free_blk_cnt, 
	   grps[i].free_inode_cnt, 
	   grps[i].useddir_cnt);
  }
}

void group_descriptors()
{
  int fd = open_file_from_input("Group descriptor");
  vector<grp_des> grps;
  
  grps = get_grp_des_from_fs(fd, _4K);
  printf("%s : %d\r\n", "Totoal group descriptors", grps.size());
  print_grp_des(grps);

  close_file_opened(fd);  
}

int main(int argc, char* argv[])
{
  int sel = 5;

  do
    {
      sel = select_menu();
      switch(sel)
	{
	case 1: calcuate_block_group(); break;
	case 2: superblock(); break;
	case 3: group_descriptors(); break;
	default:  break;
	}
    }while(sel);

  return 0;
}
