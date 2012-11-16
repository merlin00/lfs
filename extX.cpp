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

int get_superblock_from_fs(int fd, fs_superblock* superblk)
{
  if(lseek(fd, POS_OF_SUPERBLOCK, SEEK_SET) < 0) return -1;
  return read(fd, superblk, SIZE_OF_SUPERBLOCK);
}

vector<grp_des>& get_grp_des_from_fs(int fd, block_type type)
{
  _byte* fs_buf = 0;
  int fs_offset = 0, grp_offset = 0;
  size_t blk_size = (size_t)type;
  vector<grp_des> grps;
  grp_des grp;
  
  switch(type)
    {
    case _1K:
    case _2K: fs_offset = 2048; break;
    case _4K: fs_offset = 4096; break;
    }
  
  fs_buf = new _byte[(size_t)type];
  if(!fs_buf) goto END;
  if(lseek(fd, fs_offset, SEEK_SET) < 0) goto FREE_END;
  if(read(fd, fs_buf, blk_size) < 0) goto FREE_END;;
  
  while(grp_offset < blk_size) {
    memcpy(&grp, fs_buf + grp_offset, SIZE_OF_GROUP_DESCRIPTOR);
    grps.push_back(grp);
    grp_offset += SIZE_OF_GROUP_DESCRIPTOR;
  }

 Free_END:
  delete[] fs_buf;
 END:
  return grps;  
}
