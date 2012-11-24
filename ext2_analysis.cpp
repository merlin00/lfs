#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "ext2_analysis.hpp"

using namespace std;

ExtAnalysis::ExtAnalysis()
  : m_fd(0)
{
}

ExtAnalysis::ExtAnalysis(const char* filename)
  : m_fd(0)
{
  open(filename);  
}

int ExtAnalysis::_read_super_block()
{
  int ret = 0;
  if(lseek(m_fd, POS_OF_SUPERBLOCK, SEEK_SET) < 0) return -1;

  ret = read(m_fd, &m_super_block, SIZE_OF_SUPERBLOCK); 
  //Add check routine for super block.

  return ret;
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
  if(m_fd != 0 || filename == 0) return false;

  m_fd = ::open(filename, O_RDONLY);
  if(m_fd < 0) return false;

  if(_read_super_block() < 0) goto close_fd;
  if(_read_group_descs(m_super_block) < 0) goto close_fd;

  return true;
  
 close_fd:
  close();  
  return false;
}

void ExtAnalysis::close()
{
  if(m_fd != 0) {
    ::close(m_fd);
    m_fd = 0;
  }
}

// Read inode bitmap from file or dev
// by group dsecriptor and super block.
int ExtAnalysis::get_inode_bitmap(_dword group_num,
				   _byte* bitmap,
				   size_t size)
{
  _dword block_size = GET_BLOCK_SIZE(m_super_block.log_blk_size); 
  size_t min_size = get_inode_bitmap_length();
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
  _dword block_size = get_block_bitmap_length();
  off64_t offset = (off64_t)m_group_descs[group_num].inode_bitmap * block_size;

  if(size < block_size) return -1;
  memset(bitmap, 0, size);
  if(lseek64(m_fd, offset, SEEK_SET) < 0) return -1;

  return read(m_fd, bitmap, block_size);
}

void ExtAnalysis::get_used_bits(const _byte* bitmap, size_t size, vector<_dword>& used_bits)
{
  for(int i = 0 ; i < size ; i++)
    {
      _byte val = bitmap[i];
      _byte mask = 0x80;
      for(int j = 0 ; val ; j++)
	{
	  if(val & mask) {
	    val = val ^ mask;
	    used_bits.push_back((i * 8) + j);
	  }
	  mask = mask >> 1;	    
	}	     
    }
}

int ExtAnalysis::get_inodes(_dword group_num, vector<inode_info>& inodes)
{
  _byte *bitmap = 0, *buf_inodes = 0;
  _dword block_size = GET_BLOCK_SIZE(m_super_block.log_blk_size);
  _dword inode_size = m_super_block.inode_size;
  _dword buf_size = m_super_block.inode_per_group * m_super_block.inode_size;
  _dword ret, offset;
  off64_t disk_pos = (off64_t)m_group_descs[group_num].inode_table * block_size;

  vector<_dword> used_bits;

  bitmap = new _byte[block_size];

  ret = get_inode_bitmap(group_num, bitmap, block_size);
  get_used_bits(bitmap, block_size, used_bits);

  delete[] bitmap;

  if(lseek64(m_fd, disk_pos, SEEK_SET) < 0) return -1;
  buf_inodes = new _byte[buf_size];
  read(m_fd, buf_inodes, buf_size);

  size_t size = used_bits.size();
  for(int i = 0 ; i < size ; i++)
    {
      offset = used_bits[i] * inode_size;
      used_bits[i] = used_bits[i] + group_num * m_super_block.inode_per_group + 1;
      cout << used_bits[i] << endl;
      inodes.push_back(std::make_pair(used_bits[i],*((i_node*)(buf_inodes + offset))));      
    }

  delete[] buf_inodes;

  return used_bits.size();
}

i_node ExtAnalysis::get_inode(_dword ino)
{
  
  
}
