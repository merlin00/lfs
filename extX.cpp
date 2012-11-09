#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "extX.h"

#define SIZE_OF_BLOCK 4096
#define INODES_PER_GROUP 8192

int create_block(block_type type, block_des* blk)
{
  size_t size = (size_t)type;
  void *p = 0;

  // Allocate memory as size of block
  p = malloc(size);
  if(p == 0) {
    blk->size = 0; 
    blk->block = 0;
    return -1;
  }

  blk->size = size; 
  blk->block = (char*)p;
  
  return 0;
}

void delete_block(block_des* blk)
{
  if(blk->size == 0) return;

  free(blk->block);
  blk->size = 0;
  blk->block = 0;
}

int read_block(int fd, block_des* blk)
{
  if(fd < 0) return -1;
  return read(fd, blk->block, blk->size);
}
/*
int get_direct_img(int fd, dirent_disk_img* pdir)
{
  ssize_t len = 0;
  off_t offset = 0;

  len = read(fd, pdir, HDR_SIZE_OF_DIRENT);
  if(len < 0) return len;

  len = read(fd, pdir->name, pdr->name_len);
  if(len < 0) return len;
  
  offset = lseek(fd, pdir->rec_len, SEEK_CUR);
  return 0;
  }*/

#define BLOCK_GROUP_OF_INODE(inode, inodes_per_group) \
  (inode - 1) / inodes_per_group
#define ENTRY_INODE_TABLE(inode, inodes_per_group) \
  (inode - 1) % inodes_per_group

typedef struct _extX_info {
  unsigned int blks_of_group;	 // Blocks of group
  unsigned int inodes_per_group; // i-nodes per group
  unsigned short size_of_blk;	 // Size of block
  unsigned short size_of_bitmap; // Bitmap size 
}extX_info;

extX_info get_extX_info(block_type type)
{
  extX_info info;
  int size_of_blk = (int)type;
  
  info.size_of_blk = size_of_blk;
  info.size_of_bitmap = size_of_blk;
  info.blks_of_group = size_of_blk * 8; // 8 bits (1 block per bit)

  return info;
}

int main(int argc, char* argv[])
{
  int num_of_inode = 0;
  int num_of_group =0;
  int entry =0;

  printf("%-30s : ", "Input I-Node #");
  scanf("%d", &num_of_inode);
  printf("\r\n");

  num_of_group = BLOCK_GROUP_OF_INODE(num_of_inode, INODE_PER_GROUP);
  entry = ENTRY_INODE_TABLE(num_of_inode, INODE_PER_GROUP);

  printf("%-30s : %d\r\n", "i-node num", num_of_inode);
  printf("%-30s : %d\r\n", "Block group number", num_of_group);
  printf("%-30s : %d\r\n", "The offset of i-node table", entry);

  return -1;
}
