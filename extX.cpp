#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <vector>

#include "extX.h"

using namespace std;

#define POS_OF_SUPERBLOCK 1024
#define SIZE_OF_SUPERBLOCK 264

int read_super_block(int fd, fs_super_block* fs_super)
{
  if(lseek(fd, POS_OF_SUPERBLOCK, SEEK_SET) < 0) return -1;
  return read(fd, fs_super, SIZE_OF_SUPERBLOCK);
}


int read_group_desc(int fd, 
		    size_t blk_size, 
		    const fs_super_block& fs_super, 
		    vector<group_desc> &groups)
{
  _byte* fs_buf = 0;
  int fs_offset = 0, gd_offset = 0;
  int gd_cnt = 0, buf_size, ret = 0;
  
  switch(blk_size)
    {
    case _1K_BLOCK: break;
    case _2K_BLOCK: break;
    case _4K_BLOCK: break;
    default:  return -1;
    }

  fs_offset = blk_size;

  gd_cnt = fs_super.blk_cnt / fs_super.blks_per_group;  
  buf_size = gd_cnt * SIZE_OF_GROUP_DESCRIPTOR;
  fs_buf = new _byte[buf_size];

  if(!fs_buf) goto END;
  if(lseek(fd, fs_offset, SEEK_SET) < 0) goto FREE_END;

  ret = read(fd, fs_buf, buf_size);
  if(ret < buf_size) goto FREE_END;

  for(int i = 0 ; i < gd_cnt ; i ++)
    {
      groups.push_back(*((group_desc*)(fs_buf + gd_offset)));
      gd_offset += SIZE_OF_GROUP_DESCRIPTOR;
    }

  ret = gd_cnt;

 FREE_END:
  delete[] fs_buf;
 END:
  return gd_cnt;  
}

// Read inode bitmap from file or dev
// by group dsecriptor and super block.
_dword read_inode_bitmap(int fd,
			 const fs_super_block& super,
			 const group_desc& gd,
       			 _byte* bitmap,
			 size_t bitmap_size)
{
  size_t buf_size = super.inode_per_group / 8;
  off64_t offset = (off64_t)gd.inode_bitmap * GET_BLOCK_SIZE(super.log_blk_size);

  if(bitmap_size < buf_size) return -1;
  memset(bitmap, 0, bitmap_size);

  if(lseek64(fd, offset, SEEK_SET) < 0) return -1;

  return read(fd, bitmap, buf_size);
}

// Read block bitmap from file or dev, 
// by group dsecriptor and super block.
_dword read_block_bitmap(int fd,
			 const fs_super_block& super,
			 const group_desc& gd,
       			 _byte* bitmap,
			 size_t bitmap_size)
{
  size_t buf_size = GET_BLOCK_SIZE(super.log_blk_size);
  off64_t offset = (off64_t)gd.inode_bitmap * buf_size;

  if(bitmap_size < buf_size) return -1;
  memset(bitmap, 0, bitmap_size);

  if(lseek64(fd, offset, SEEK_SET) < 0) return -1;

  return read(fd, bitmap, buf_size);
}

//Find out vaild offset from bitmap.
void get_vaild_offset(const _byte* bitmap,
		      size_t bitmap_size,
		      vector<_dword>& offset)
{
  for(int i = 0 ; i < bitmap_size ; i++)
    {
      _byte val = bitmap[i];
      _byte mask = 0x80;
      for(int j = 0 ; val ; j++)
	{
	  if(val & mask) {
	    val = val ^ mask;
	    offset.push_back((i * 8) + j);
	  }
	  mask = mask >> 1;	    
	}	     
    }
}


_dword read_vaild_inodes(int fd,
			 const fs_super_block& super,
			 const group_desc& gd,
			 vector<i_node>& inodes)
{
  _byte *bitmap = 0, *buf_inodes = 0;
  _dword blk_size = GET_BLOCK_SIZE(super.log_blk_size);
  _dword buf_size = super.inode_per_group * super.inode_size;
  _dword ret, pos;
  off64_t disk_offset =  (off64_t)gd.inode_table * (off64_t)blk_size;
  vector<_dword> offset;
  
  bitmap = new _byte[blk_size];

  ret = read_inode_bitmap(fd, super, gd, bitmap, blk_size);
  get_vaild_offset(bitmap, blk_size, offset);

  delete[] bitmap;

  printf("%u,%u,%u\r\n", gd.inode_table, blk_size, disk_offset);

  lseek64(fd, disk_offset, SEEK_SET);
  buf_inodes = new _byte[buf_size];
  read(fd, buf_inodes, buf_size);

  size_t size = offset.size();
  for(int i = 0 ; i < size ; i++)
    {
      pos = offset[i] * super.inode_size;
      inodes.push_back(*((i_node*)(buf_inodes + pos)));      
    }

  delete[] buf_inodes;

  return size;
}

