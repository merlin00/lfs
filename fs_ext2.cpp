/***************************************************************/
/*
  Copyright (C) 2012  Kwon kideok (merlin00@pusan.ac.kr)
  Ubiqutious Computing Laboratory in Pusan National University

  2012.12.03  

 */
/***************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "ext2.h"
#include "fs_ext2.hpp"

using namespace std;

FsExt2::FsExt2(const char* filename)
{
  open(filename);  
}

int FsExt2::_read_super_block()
{
  int ret = 0;
  if(lseek(m_fd, POS_OF_SUPERBLOCK, SEEK_SET) < 0) return -1;

  ret = read(m_fd, &m_super_block, SIZE_OF_SUPERBLOCK); 
  //Add check routine for super block.

  return ret;
}

int FsExt2::_read_group_descs(const ext_super_block& super)
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

bool FsExt2::open(const char* filename)
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

void FsExt2::close()
{
  if(m_fd != 0) {
    ::close(m_fd);
    m_fd = 0;
  }
}

// Read inode bitmap from file or dev
// by group dsecriptor and super block.
int FsExt2::get_inode_bitmap(_dword group_num,
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
int FsExt2::get_block_bitmap(_dword group_num,
				  _byte* bitmap,
				  size_t size)
{
  _dword block_size = get_block_bitmap_length();
  off64_t offset = (off64_t)m_group_descs[group_num].block_bitmap * block_size;

  if(size < block_size) return -1;
  memset(bitmap, 0, size);
  if(lseek64(m_fd, offset, SEEK_SET) < 0) return -1;

  return read(m_fd, bitmap, block_size);
}

// Return used bits in bitmap
void FsExt2::get_used_bits(const _byte* bitmap, size_t size, vector<_dword>& used_bits)
{
  for(int i = 0 ; i < size ; i++)
    {
      _byte val = bitmap[i];
      _byte mask = 0x80;

      // Find bit number from byte.
      for(int j = 0 ; val ; j++)
	{
	  if(val & mask) {
	    val = val ^ mask;
	    used_bits.push_back((i * 8) + j);
	  }
	  // Shift bit mask
	  mask = mask >> 1;	    
	}	     
    }
}

int FsExt2::get_inodes(_dword group_num, vect_inodes& inodes)
{
  _byte *bitmap = 0, *buf_inodes = 0;
  _dword block_size, inode_size, buf_size, offset;
  size_t size;
  off64_t disk_pos;

  vector<_dword> used_bits;

  // the block_size of current file system
  block_size = GET_BLOCK_SIZE(m_super_block.log_blk_size);
  inode_size = m_super_block.inode_size;
  // Total size of inode table in current disk
  buf_size = m_super_block.inode_per_group * m_super_block.inode_size;

  // Calculate the block position of inode table in current disk.
  disk_pos = (off64_t)m_group_descs[group_num].inode_table * block_size;

  bitmap = new _byte[block_size];
  if(!bitmap) return 0;

  // Find used bits for inode number.
  if(get_inode_bitmap(group_num, bitmap, block_size) < 0) goto free_bitmap;
  get_used_bits(bitmap, block_size, used_bits);

  // Move the block position of indoe table.
  if(lseek64(m_fd, disk_pos, SEEK_SET) < 0) goto free_bitmap;

  buf_inodes = new _byte[buf_size];
  if(!buf_inodes) goto free_bitmap;
  if(read(m_fd, buf_inodes, buf_size) < buf_size) goto all_free;

  // Search vaild inodes with used bits.
  size = used_bits.size();
  for(int i = 0 ; i < size ; i++)
    {
      offset = used_bits[i] * inode_size;
      // Plus 1 because inode number starts from 1. 
      used_bits[i] = used_bits[i] + group_num * m_super_block.inode_per_group + 1;
      inodes.push_back(std::make_pair(used_bits[i],*((i_node*)(buf_inodes + offset))));      
    }

 all_free:
  delete[] buf_inodes;
 free_bitmap:
  delete[] bitmap;

  return used_bits.size();
}

i_node FsExt2::get_inode(_dword ino)
{
  // inode sequence starts from 1;
  // if(ino > 0) throw 0;
  i_node inode;
  _dword inode_size = m_super_block.inode_size;
  const group_desc& gd = m_group_descs[get_group_number(ino)];
  off64_t offset = (off64_t)gd.inode_table * GET_BLOCK_SIZE(m_super_block.log_blk_size);
  offset += get_inode_offset(ino) * inode_size;

  lseek64(m_fd, offset, SEEK_SET);
  memset(&inode, 0, 128);
  read(m_fd, &inode, 128);

  return inode;  
}

int FsExt2::get_block(_dword block_num, _byte* block, size_t size)
{
  _dword block_size = get_block_size();
  off64_t offset = (off64_t)block_size * block_num;

  if(size < block_size) return -1;
  lseek64(m_fd, offset, SEEK_SET);

  return read(m_fd, block, block_size);
}


int FsExt2::get_dir_entries(const _byte* block, 
			    size_t size,
			    vector<ext2_dir_entry>& dir_entries)
{
  _dword offset = 0;
  ext2_dir_entry dir;

  while(offset < size) {
    memset(&dir, 0, sizeof(ext2_dir_entry));
    memcpy(&dir, block + offset, SIZE_OF_EXT2_DIR);
    memcpy(dir.name, block + offset + SIZE_OF_EXT2_DIR, dir.name_len);
    offset += dir.rec_len;
    dir_entries.push_back(dir);    
  }

  return dir_entries.size();	 
}


