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

int get_super_block_from(int fd, fs_super_block* fs_super)
{
  if(lseek(fd, POS_OF_SUPERBLOCK, SEEK_SET) < 0) return -1;
  return read(fd, fs_super, SIZE_OF_SUPERBLOCK);
}


int get_group_desc_from(int fd, 
			size_t blk_size, 
			const fs_super_block& fs_super, 
			vector<group_desc> &groups)
{
  _byte* fs_buf = 0;
  int fs_offset = 0, gd_offset = 0;
  int gd_cnt, buf_size;
  group_desc gd;
  
  switch(blk_size)
    {
    case _1K_BLOCK: break;
    case _2K_BLOCK: break;
    case _4K_BLOCK: break;
    default:  return -1;
    }

  gd_cnt = fs_super.blk_cnt / fs_super.blks_per_group;
  fs_offset = blk_size;
  
  buf_size = gd_cnt * SIZE_OF_GROUP_DESCRIPTOR;
  fs_buf = new _byte[buf_size];

  if(!fs_buf) goto END;
  if(lseek(fd, fs_offset, SEEK_SET) < 0) goto FREE_END;
  if(read(fd, fs_buf, buf_size) < 0) goto FREE_END;

  for(int i = 0 ; i < gd_cnt ; i ++)
    {
       memcpy(&gd, fs_buf + gd_offset, SIZE_OF_GROUP_DESCRIPTOR);
       groups.push_back(gd);
       gd_offset += SIZE_OF_GROUP_DESCRIPTOR;
    }

 FREE_END:
  delete[] fs_buf;
 END:
  return gd_cnt;  
}
