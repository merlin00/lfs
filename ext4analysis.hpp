#ifndef _EXT4ANALYSIS_H_
#define _EXT4ANALYSIS_H_

#include "extX.h"

class Ext4Analysis
{
private:
  int m_fd;
  ext_super_block m_super_block;
  std::vector<group_desc> m_group_descs;

  int _read_super_block();

  int _read_group_descs(const ext_super_block& super);

public:
  typedef std::pair<_dword, i_node> pair_inode;
  typedef std::vector<pair_inode> vect_inodes;
  typedef std::vector<ext2_dir_entry> vect_dir;

  inline _dword get_group_number(_dword ino) {
    // if(ino > 0) 
    return (ino - 1) / m_super_block.inode_per_group;
  }

  inline _dword get_inode_offset(_dword ino) {
    return (ino - 1) % m_super_block.inode_per_group;
  }

public:
  Ext4Analysis();
  ~Ext4Analysis() { close(); }
  Ext4Analysis(const char* filename);

  bool open(const char* filename);
  void close();

  const ext_super_block& get_super_block() { 
    return m_super_block;
  }
  const std::vector<group_desc>& get_group_desc() {
    return m_group_descs;
  }

  inline int get_inode_bitmap_length() {
    return m_super_block.inode_per_group / 8;
  }
  
  inline int get_block_bitmap_length() {
    return  GET_BLOCK_SIZE(m_super_block.log_blk_size); 
  }

  inline _dword get_block_size() {
    return ((1 << m_super_block.log_blk_size) * _1K_BLOCK);
  }

  int get_inode_bitmap(_dword group_num, 
			_byte* bitmap,
			size_t size);
  int get_block_bitmap(_dword group_num,
		       _byte* bitmap,
		       size_t size);

  void get_used_bits(const _byte* bitmap, 
		     size_t size, 
		     std::vector<_dword>& used_bits);
  int get_inodes(_dword group_num, vect_inodes& inodes);

  i_node get_inode(_dword ino);
  int get_block(_dword block_num, _byte* block, size_t size);

  ext4_extent_header get_block_tree(const char* block, 
				    size_t size, 
				    std::vector<ext4_extent>& entries);

  static int extract_dir_from_block(const _byte* block, size_t size, vect_dir& dir);

};

#endif
