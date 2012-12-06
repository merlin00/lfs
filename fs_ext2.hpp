/***************************************************************/
/*
  Copyright (C) 2012 Kwon kideok (merlin00@pusan.ac.kr)
  Ubiqutious Computing Laboratory in Pusan National University

  2012.12.03  

 */
/***************************************************************/
#ifndef _FS_EXT2_H_
#define _FS_EXT2_H_

#define EXT2_NAME_LEN 256
#define SIZE_OF_EXT2_DIR 8

typedef struct _ext2_dir_entry {
  _dword inode;
  _word rec_len;
  _byte name_len;
  _byte file_type;
  char name[EXT2_NAME_LEN];
}ext2_dir_entry;

// FsExt2 class
class FsExt2
{

protected:
  int m_fd;
  ext_super_block m_super_block;
  std::vector<group_desc> m_group_descs;

  int _read_super_block();
  int _read_group_descs(const ext_super_block& super);

public:  
  typedef std::pair<_dword, i_node> pair_inode;
  typedef std::vector<pair_inode> vect_inodes;
  //  typedef std::vector<ext2_dir_entry> vect_dir;
  
  inline _dword get_group_number(_dword ino) {
    // if(ino > 0) 
    return (ino - 1) / m_super_block.inode_per_group;
  }

  inline _dword get_inode_offset(_dword ino) {
    return (ino - 1) % m_super_block.inode_per_group;
  }

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

public:
  FsExt2() : m_fd(0) {}
  FsExt2(const char* filename);
  ~FsExt2() { close(); }

  bool open(const char* filename);
  void close();
  
  int get_inode_bitmap(_dword group_num, _byte* bitmap,	size_t size);
  int get_block_bitmap(_dword group_num, _byte* bitmap, size_t size);

  void get_used_bits(const _byte* bitmap, 
		     size_t size, 
		     std::vector<_dword>& used_bits);
  int get_inodes(_dword group_num, vect_inodes& inodes);

  i_node get_inode(_dword ino);
  int get_block(_dword block_num, _byte* block, size_t size);

  int get_dir_entries(const _byte* block, 
		      size_t size, 
		      std::vector<ext2_dir_entry>& dir_entries);

  //  static int extract_dir_from_block(const _byte* block, size_t size, vect_dir& dir);
};

#endif
