#ifndef _EXT4PRESENTATION_HPP_

#define _EXT2PRESENTATION_HPP_

class Ext4Presentation
{
private:
  Ext4Analysis* m_ext4;

  void _print_inode(_dword ino, const i_node& inode);
  void _print_inodes_in_group(const Ext4Analysis::vect_inodes& inodes);
  void _print_title_of_group_desc();
  void _print_element_of_group_desc(const std::vector<group_desc>& groups);
  void _print_symbolic_link(const i_node& inode);
  void _print_extent_header(const ext4_extent_header& hdr);

  void _dump(const _byte* buf, size_t size);

public:
  Ext4Presentation();
  ~Ext4Presentation();

  void attach_ext4(Ext4Analysis* ext4) { m_ext4 = ext4; }
  Ext4Analysis* detach_ext4() { return m_ext4; }

  void print_super_block(); 
  void print_group_desc();
  void print_inodes_in_group(_dword group_num);
  void print_inode(_dword ino);
  void dump_block(_dword num);

  void print_inode_map(_dword group_num);
  void print_block_map(_dword group_num);
 
};

#endif

