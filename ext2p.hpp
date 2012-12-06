/***************************************************************/
/*
  Copyright (C) 2012 Kwon kideok (merlin00@pusan.ac.kr)
  Ubiqutious Computing Laboratory in Pusan National University

  2012.12.03  

 */
/***************************************************************/
#ifndef _EXT2P_HPP_
#define _EXT2P_HPP_

class Ext2p
{
private:
  FsExt2* m_ext;
  bool m_dump;

  void _print_inode(_dword ino, const i_node& inode);
  void _print_inodes_in_group(const FsExt2::vect_inodes& inodes);
  void _print_title_of_group_desc();
  void _print_element_of_group_desc(const std::vector<group_desc>& groups);
  void _print_symbolic_link(const i_node& inode);
  void _print_dir_entries(_dword block_num);
  void _print_regfile_block(const i_node& inode);

  void _dump(const _byte* buf, size_t size);

public:
  Ext2p();
  ~Ext2p();

  void set_dump() { m_dump = true; }
  void clear_dump() { m_dump = false; }

  void attach(FsExt2* ext) { m_ext = ext; }
  FsExt2* detach() { return m_ext; }

  void print_super_block(); 
  void print_group_desc();
  void print_inodes_in_group(_dword group_num);
  void print_inode(_dword ino);
  void dump_block(_dword num);

  void print_inode_map(_dword group_num);
  void print_block_map(_dword group_num);
};

#endif

