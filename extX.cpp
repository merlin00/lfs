#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include <string.h>
#include <arpa/inet.h>

#include "extX.h"

#define SIZE_OF_BLOCK 4096
#define MAGIC_OFFSET_OF_SUPERBLOCK 0x38

#define INODES_PER_GROUP 8192

typedef enum _fs_data_type {
  FS_PADDING,
  FS_BYTE,
  FS_WORD,
  FS_DWORD
}fs_data_type;

typedef struct _fs_data_struct{
  fs_data_type type;
  _dword len;
}fs_data_struct;

static fs_data_struct SUPERBLOCK_FIELDS[] = {
  {FS_DWORD, 4}, /* [4 bytes] Inode count */
  {FS_DWORD, 4}, /* [4 bytes] block count */
  {FS_DWORD, 4}, /* [4 bytes] res block count */
  {FS_DWORD, 4}, /* [4 bytes] Free block count */
  {FS_DWORD, 4}, /* [4 bytes] Free inode count */
  {FS_DWORD, 4}, /* [4 bytes] First data node block */
  {FS_DWORD, 4}, /* [4 bytes] Log block size */
  {FS_DWORD, 4}, /* [4 bytes] Log frag size */
  {FS_DWORD, 4}, /* [4 bytes] Block per group */
  {FS_DWORD, 4}, /* [4 bytes] Frag per group */
  {FS_DWORD, 4}, /* [4 bytes] Inode per group */
  {FS_DWORD, 4}, /* [4 bytes] mtime */
  {FS_DWORD, 4}, /* [4 bytes] wtime */
  {FS_WORD, 2},	 /* [2 bytes] Mount count */
  {FS_WORD, 2},	 /* [2 bytes] Max mount count */
  {FS_WORD, 2},	 /* [2 bytes] Magic */
  {FS_WORD, 2},	 /* [2 bytes] State */
  {FS_WORD, 2},  /* [3 bytes] Error */
  {FS_WORD, 2},  /* [3 bytes] Minor version */
  {FS_DWORD, 4}, /* [4 bytes] Last check */
  {FS_DWORD, 4}, /* [4 bytes] Check interval */
  {FS_DWORD, 4}, /* [4 bytes] Creator OS */
  {FS_DWORD, 4}, /* [4 bytes] Major version */
  {FS_WORD, 2},	 /* [2 bytes] def_res uid */
  {FS_WORD, 2},	 /* [2 bytes] def_res gid */
  {FS_DWORD, 4}, /* [4 bytes] First ino */
  {FS_WORD, 2},	 /* [2 bytes] Inode size */
  {FS_WORD, 2},	 /* [2 bytes] Block group number */
  {FS_DWORD, 4}, /* [4 bytes] Feature compat */
  {FS_DWORD, 4}, /* [4 bytes] Feature incompat */
  {FS_DWORD, 4}, /* [4 bytes] feature read only compat */
  {FS_BYTE, 16}, /* [16 bytes] UUID */
  {FS_BYTE, 16}, /* [16 bytes] Valume Name */
  {FS_BYTE, 64}, /* [64 bytes] Last mounted */
  {FS_DWORD, 4}, /* [4 bytes] Algorithm usage bitmap */
  {FS_BYTE, 1},	 /* [1 bytes] prealloc block */
  {FS_BYTE, 1},	 /* [1 bytes] Prealloc dir block */ 
  {FS_PADDING, 2}, /* [2 bytes] Padding */
  {FS_BYTE, 16}, /* [16 bytes] Journal uuid */
  {FS_DWORD, 4}, /* [4 bytes] Jouranl inode number */
  {FS_DWORD, 4}, /* [4 bytes] Journal dev */
  {FS_DWORD, 4}, /* [4 bytes] Last orphan  */
  {FS_BYTE, 16}, /* [16 bytes] Hash seed */
  {FS_BYTE, 1},	 /* [1 bytes] Hash version and padding */
  {FS_PADDING, 3}, /* [3 bytes] padding  */
  {FS_DWORD, 4}, /* [4 bytes] Default mount option */
  {FS_DWORD, 4}, /* [4 bytes] First meta block group */  
};

// Group descriptor
static fs_data_struct GROUP_DESCRIPTOR_FILEDS[] ={
  {FS_DWORD, 4},     // [4 bytes] Block bitmap
  {FS_DWORD, 4},     // [4 bytes] Inode bitmap
  {FS_DWORD, 4},     // [4 bytes] Inode table
  {FS_WORD, 2},	     // [2 bytes] Free block count
  {FS_WORD, 2},	     // [2 bytes] Free inode count
  {FS_WORD, 2},	     // [2 bytes] Used dir conut
  {FS_PADDING, 2},   // [2 bytes] Padding
  {FS_PADDING, 12}   // ]12 bytes] Padding
};

//
static fs_data_struct I_NODE_FILEDS[] = {
  {FS_WORD, 2},
};

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

int _get_struct_from_block(block_des* blk, 
			   fs_data_struct* fs_data, 
			   size_t fs_data_len,
			   _byte *dset, 
			   size_t dest_len)
{
  int src_offset = 0;
  int dest_offset = 0;
  size_t tot_len = 0;
  _byte *src = (_byte*)blk->block;

  for(int i = 0 ;  i < fs_data_len ; i++)
    if(fs_data[i].type != FS_PADDING)
      tot_len += fs_data[i].len;     

  if(tot_len > dest_len) return -1;

  for(int i = 0 ; i < fs_data_len ; i++)
    {
      int len = fs_data[i].len;

      switch(fs_data[i].type)
	{
	case FS_BYTE:
	  memcpy((void*)(dset + dest_offset), (void*)(src + src_offset), len);  
	  dest_offset += len; 
	  break;
	case FS_WORD:
	  *((_word*)(dset + dest_offset)) = ntohs(*((_word*)(src + src_offset)));
	  dest_offset += len;
	  break;
	case FS_DWORD:
	  *((_dword*)(dset + dest_offset)) = ntohl(*((_dword*)(src + src_offset)));
	  dest_offset += len;
	  break;
	case FS_PADDING:  break;
	}
      src_offset += len;
    }

  return 0;
}

int get_superblockfrom(int fd, block_type type, superblock* superblk)
{
  int offset = 0;
  int ret = 0;
  unsigned int sig = 0;
  block_des blk;
  
  ret = create_block(type, &blk);
  if(ret < 0) return ret;
  ret = read_block(fd, &blk);
  if(ret < 0) return ret;

  // According to block size, find super block.
  switch(type)
  {
  case _1K: ret = read_block(fd, &blk); break;
  case _2K:
  case _4K: offset = 1024; break;
  }
  if(ret < 0) return ret;

  // Check magic number of super block.
  sig = *(blk.block + MAGIC_OFFSET_OF_SUPERBLOCK);
  sig = (sig << 4) | *(blk.block + MAGIC_OFFSET_OF_SUPERBLOCK + 1);
  if(sig != MAGIC_NUM_OF_SUPERBLOCK)  return -1;

  // Get super block struct from the block.
  ret = _get_struct_from_block(&blk, 
			       SUPERBLOCK_FILEDS, 
			       sizeof(SUPERBLOCK_FILEDS)/sizeof(fs_data_struct),
			       (_byte*)superblk,
			       sizeof(superblk));
  delete_block(&blk);

  return ret;
}

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

  num_of_group = BLOCK_GROUP_OF_INODE(num_of_inode, INODES_PER_GROUP);
  entry = ENTRY_INODE_TABLE(num_of_inode, INODES_PER_GROUP);

  printf("%-30s : %d\r\n", "i-node num", num_of_inode);
  printf("%-30s : %d\r\n", "Block group number", num_of_group);
  printf("%-30s : %d\r\n", "The offset of i-node table", entry);

  return -1;
}
