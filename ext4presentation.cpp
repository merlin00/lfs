#include <iostream>
#include <vector>
#include <stdio.h>
#include <iomanip>

#include "extX.h"
#include "ext4analysis.hpp"
#include "ext4presentation.hpp"

using namespace std;

Ext4Presentation::Ext4Presentation() 
  : m_ext4(0)
{
}

Ext4Presentation::~Ext4Presentation()
{
}

void Ext4Presentation::print_super_block()
{
  if(!m_ext4) return;
  
  const ext_super_block& sb = m_ext4->get_super_block();
  
  cout << uppercase;
  cout << setfill(' ');

  cout << setw(22) << left << "Last mounted on" << ":"
       << setw(10) << right << sb.last_mounted << endl;
  
  cout << setw(22) << left << "Inode count" << ":"
       << setw(10) << right << sb.inode_cnt << endl;

  cout << setw(22) << left << "Block count" << ":"
       << setw(10) << right << sb.blk_cnt << endl;
	
  cout << setw(22) << left << "Res block count" << ":"
       << setw(10) << right << sb.res_blk_cnt << endl;	

  cout << setw(22) << left << "Free block count" << ":"
       << setw(10) << right << sb.free_blk_cnt << endl;

  cout << setw(22) << left << "Free inode count" << ":"
       << setw(10) << right << sb.free_inode_cnt << endl;

  cout << setw(22) << left << "First data node block" << ":"
       << setw(10) << right << sb.first_data_blk << endl;

  cout << setw(22) << left << "Block size" << ":"
       << setw(10) << right << GET_BLOCK_SIZE(sb.log_blk_size) << endl;

  cout << setw(22) << left << "Fragment size" << ":"
       << setw(10) << right << GET_BLOCK_SIZE(sb.log_frag_size) << endl;

  cout << setw(22) << left << "Blocks per group" << ":"
       << setw(10) << right << sb.blks_per_group << endl;	

  cout << setw(22) << left << "Fragments per group" << ":"
       << setw(10) << right << sb.frags_per_group << endl;	

  cout << setw(22) << left << "Inode per group" << ":"
       << setw(10) << right << sb.inode_per_group << endl;

  cout << setw(22) << left << "Mount count"<< ":"
       << setw(10) << right << sb.mount_cnt << endl;

  cout << setw(22) << left << "Max mount count" << ":"
       << setw(10) << right << (_dword)sb.max_mount_cnt << endl;
  
  cout << hex;
  cout << setw(22) << left << "Magic" << ":"
       << setw(6) << right << "0x" << setw(4) << sb.magic << endl;
  cout << dec;

  cout << setw(22) << left << "Block group #" << ":"
       << setw(10) << right << (_dword)sb.blk_group_num << endl;	

  cout << setw(22) << left << "Inode size" << ":"
       << setw(10) << right << sb.inode_size << endl;
}


void Ext4Presentation::_print_element_of_group_desc(const vector<group_desc>& groups)
{
  // Print group descriptor
  size_t size = groups.size();

  for(size_t i = 0 ; i < size ; i++) {
    printf("%3d  %9u   %9u   %9u   %7u   %7u   %7u \r\n",
	   i, 
	   groups[i].block_bitmap, 
	   groups[i].inode_bitmap, 
	   groups[i].inode_table,
	   groups[i].free_blk_cnt, 
	   groups[i].free_inode_cnt, 
	   groups[i].useddir_cnt);
  }
}

void Ext4Presentation::_print_title_of_group_desc() 
{
  // Print titile
  printf("%s\r\n", "======================================================================");
  printf("%3s  %10s  %10s  %10s  %8s  %8s  %8s\r\n",
	 "no", "blk map", "ino map", "ino table",
	 "free blk", "free ino", "used dir");
  printf("%s\r\n", "----------------------------------------------------------------------");
}

void Ext4Presentation::print_group_desc()
{
  _print_title_of_group_desc();
  _print_element_of_group_desc(m_ext4->get_group_desc());
}

void Ext4Presentation::print_inodes_in_group(_dword group_num)
{
  Ext4Analysis::vect_inodes inodes;

  m_ext4->get_inodes(group_num, inodes);  
  _print_inodes_in_group(inodes);
}

void Ext4Presentation::_print_inodes_in_group(const Ext4Analysis::vect_inodes &inodes)
{
  // Print group descriptor
  size_t size = inodes.size();

  for(size_t i = 0 ; i < size ; i++) {
    const Ext4Analysis::pair_inode& inode = inodes[i];
    printf("%3u  %04X   %04X   %10u   %04X   %10u   %10u \r\n",
	   inode.first, 
	   inode.second.mode, 
	   inode.second.uid, 
	   inode.second.size,
	   inode.second.gid, 
	   inode.second.link_cnt, 
	   inode.second.blocks);
    }
}

void Ext4Presentation::_print_inode(_dword ino, const i_node& inode)
{
  cout << setw(15) << left << "inode #" << setw(10) << right << ino << endl;
  cout << uppercase;
  cout << oct;
  cout << setw(15) << left << "mode" << setw(4) << right << "o" << setw(6) << right << inode.mode << endl;
  cout << dec;
  cout << setw(15) << left << "uid" << setw(10) << right << inode.uid << endl;
  cout << setw(15) << left << "size" << setw(10) << right << inode.size << endl;
  cout << setw(15) << left << "gid" << setw(10) << right << inode.gid << endl;
  cout << setw(15) << left << "link count" << setw(10) << right << inode.link_cnt << endl;
  cout << setw(15) << left << "blocks" << setw(10) << right << inode.blocks << endl;
  cout << hex;
  cout << setw(15) << left << "flags" << setw(10) << right << inode.flags << endl;
  cout << dec;

  for(int i = 0 ; i < 15 ; i++) {
    cout << "block[" << setw(2) << i << "]" << setw(6) << ""
	 << setw(10) << right << inode.block[i] << endl;
  }
  cout << setfill(' ');
}

void Ext4Presentation::print_inode(_dword ino)
{
  i_node inode = m_ext4->get_inode(ino);
  _print_inode(ino, inode);
}

void Ext4Presentation::_dump(const _byte* buf, size_t size)
{
  int col = 16;
  int line = size / col;

  // Display dump title
  cout << uppercase;
  cout << hex;

  cout << setw(5) << setfill(' ') << " ";
  for(int i = 0 ; i < col ; i++)
    cout << setw(2) << setfill('0') << hex << i << " ";
  cout << setw(13) << setfill(' ') << "ASCII code" << endl; 
  cout << setw(68) << setfill('-') << "" <<endl;
  
  for(int i = 0 ; i < line ; i++) {
    int offset = i * col;
    
    cout << setw(3) << setfill('0') << offset << "  ";

    // Hex value
    for(int j = 0 ; j < col ; j++) 
      cout << setw(2) << setfill('0') << hex << (_dword)(buf[offset + j]) << " ";
          
    // ASCII code
    for(int j = 0 ; j < col ; j++) {
      char ch = buf[offset + j];
      if(ch > 31 && ch < 127) cout << setw(1) << (char)(buf[offset + j]);
      else cout << setw(1) << ".";
    }
    cout << endl;      
  }
  cout << dec << setfill(' ');
  
}

void Ext4Presentation::dump_block(_dword num)
{
  _byte* buf = 0;
  _dword block_size = m_ext4->get_block_size();
  
  buf = new _byte[block_size];
  if(!buf) return;

  // Search out block from file system.
  m_ext4->get_block(num, buf, block_size);

  cout << "Block # " << num << endl;
  _dump(buf, block_size);

  delete[] buf;
}

void Ext4Presentation::print_inode_map(_dword group_num)
{
  _byte* buf = 0;
  _dword block_size = m_ext4->get_block_size();

  buf = new _byte[block_size];
  if(!buf) return;

  int retval = m_ext4->get_inode_bitmap(group_num, buf, block_size);

  _dump(buf, retval);

  delete[] buf;
}

void Ext4Presentation::print_block_map(_dword group_num)
{
  _byte* buf = 0;
  _dword block_size = m_ext4->get_block_size();

  buf = new _byte[block_size];
  if(!buf) return;

  int retval = m_ext4->get_block_bitmap(group_num, buf, block_size);

  _dump(buf, retval);

  delete[] buf;
}
