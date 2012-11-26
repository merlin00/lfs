#ifndef _EXT2PRESENTATION_HPP_

#define _EXT2PRESENTATION_HPP_

class Ext2Presentation
{
private:
  Ext2Analysis* m_ext2;

  void _print_inode(_dword ino, const i_node& inode);
  void _print_inodes_in_group(const Ext2Analysis::vect_inodes& inodes);
  void print_title_of_group_desc();
  void print_element_of_group_desc(const std::vector<group_desc>& groups);

public:
  Ext2Presentation();
  ~Ext2Presentation();

  void attach_ext2(Ext2Analysis* ext2) { m_ext2 = ext2; }
  Ext2Analysis* detach_ext2() { return m_ext2; }

  void print_super_block(); 
  void print_group_desc();
  void print_inodes_in_group(_dword group_num);
  void print_inode(_dword ino);
 
};

#endif

