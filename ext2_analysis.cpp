#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "ext2_analysis.h"

using namespace std;

ExtAnalysis::ExtAnalysis()
  : m_fd(0)
{
}

ExtAnalysis::ExtAnalysis(const char* filename)
{
  // open(filename);  
}

int ExtAnalysis::_read_super_block()
{
  if(lseek(m_fd, POS_OF_SUPERBLOCK, SEEK_SET) < 0) return -1;
  return read(m_fd, &m_super_block, SIZE_OF_SUPERBLOCK); 
}

int ExtAnalysis::_read_group_descs(const ext_super_block& super)
{
  _byte* buf = 0;
  int gd_pos = 0, gd_offset = 0;
  int gd_cnt = 0, buf_size, ret = 0;
  _dword block_size = GET_BLOCK_SIZE(super.log_blk_size);

  if(block_size == _1K_BLOCK) gd_pos = _1K_BLOCK;
  gd_pos += block_size;

  gd_cnt = super.blk_cnt / super.blks_per_group;  
  buf_size = gd_cnt * SIZE_OF_GROUP_DESCRIPTOR;
  buf = new _byte[buf_size];

  if(!buf) goto END;
  if(lseek(m_fd, gd_pos, SEEK_SET) < 0) goto FREE_END;
  if(read(m_fd, buf, buf_size) < buf_size) goto FREE_END;

  for(int i = 0 ; i < gd_cnt ; i ++)
    {
      m_group_descs.push_back(*((group_desc*)(buf + gd_offset)));
      gd_offset += SIZE_OF_GROUP_DESCRIPTOR;
    }

  ret = gd_cnt;

 FREE_END:
  delete[] buf;
 END:
  return ret;   
}


bool ExtAnalysis::open(const char* filename)
{
  if(m_fd == 0) {
    m_fd = ::open(filename, O_RDONLY);
    return true;
  }

  return false;
}

void ExtAnalysis::close()
{
  if(m_fd != 0) ::close(m_fd);
}

// Read inode bitmap from file or dev
// by group dsecriptor and super block.
int ExtAnalysis::get_inode_bitmap(_dword group_num,
				   _byte* bitmap,
				   size_t size)
{
  _dword block_size = GET_BLOCK_SIZE(m_super_block.log_blk_size); 
  size_t min_size = m_super_block.inode_per_group / 8;
  off64_t offset = (off64_t)m_group_descs[group_num].inode_bitmap * block_size;

  if(size < min_size) return -1;
  memset(bitmap, 0, size);
  if(lseek64(m_fd, offset, SEEK_SET) < 0) return -1;

  return read(m_fd, bitmap, min_size);
}

// Read block bitmap from file or dev, 
// by group dsecriptor and super block.
int ExtAnalysis::get_block_bitmap(_dword group_num,
				  _byte* bitmap,
				  size_t size)
{
  _dword block_size = GET_BLOCK_SIZE(m_super_block.log_blk_size); 
  off64_t offset = (off64_t)m_group_descs[group_num].inode_bitmap * block_size;

  if(size < block_size) return -1;
  memset(bitmap, 0, size);
  if(lseek64(m_fd, offset, SEEK_SET) < 0) return -1;

  return read(m_fd, bitmap, block_size);
}
