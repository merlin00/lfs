/***************************************************************/
/*
  Copyright (C) 2012 Kwon kideok (merlin00@pusan.ac.kr)
  Ubiqutious Computing Laboratory in Pusan National University
n
  2012.12.03  

 */
/***************************************************************/
#include <iostream>
#include <vector>
#include <iomanip>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "ext2.h"
#include "fs_ext2.hpp"
#include "ext2p.hpp"

using namespace std;

Ext2p::Ext2p() 
  : m_ext(0)
  , m_dump(false)
{
}

Ext2p::~Ext2p()
{
}

void Ext2p::print_super_block()
{
  if(!m_ext) return;
  
  const ext_super_block& sb = m_ext->get_super_block();
  
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

void Ext2p::_print_element_of_group_desc(const vector<group_desc>& groups)
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

void Ext2p::_print_title_of_group_desc() 
{
  // Print titile
  printf("%s\r\n", "======================================================================");
  printf("%3s  %10s  %10s  %10s  %8s  %8s  %8s\r\n",
	 "no", "blk map", "ino map", "ino table",
	 "free blk", "free ino", "used dir");
  printf("%s\r\n", "----------------------------------------------------------------------");
}

void Ext2p::print_group_desc()
{
  _print_title_of_group_desc();
  _print_element_of_group_desc(m_ext->get_group_desc());
}

void Ext2p::_print_symbolic_link(const i_node& inode)
{
  if(inode.size < 60) {
    if(m_dump) {
      cout << "i_blocks hex code" << endl;
      _dump((_byte*)inode.block, 60);
      cout << endl;
    }

    cout << "i_blocks used as Symbolic link path if its length less than 60 bytes" << endl;
    cout << "Symbolic link path : ";
    cout << (char*)inode.block << endl << endl;
  }
  else {
    _print_regfile_block(inode);
  }
}

void Ext2p::print_inodes_in_group(_dword group_num)
{
  FsExt2::vect_inodes inodes;

  m_ext->get_inodes(group_num, inodes);  
  _print_inodes_in_group(inodes);
}

void Ext2p::_print_inodes_in_group(const FsExt2::vect_inodes &inodes)
{
  // Print group descriptor
  size_t size = inodes.size();

  for(size_t i = 0 ; i < size ; i++) {
    const FsExt2::pair_inode& inode = inodes[i];
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

void Ext2p::_print_inode(_dword ino, const i_node& inode)
{
  cout << "File type ";
  if(S_ISLNK(inode.mode)) cout << "[Symbolic link]" << endl;
  else if(S_ISDIR(inode.mode)) cout << "[Directory file]" << endl;   
  else if(S_ISREG(inode.mode)) cout << "[Reqular file]" << endl;
  else if(S_ISCHR(inode.mode)) cout << "[Character device]" << endl;
  else if(S_ISBLK(inode.mode)) cout << "[Block device]" << endl;
  else if(S_ISFIFO(inode.mode)) cout << "[FIFO]" << endl;
  else if(S_ISSOCK(inode.mode)) cout << "[Socket]" << endl;

  cout << setw(15) << left << "inode #" << setw(10) << right << ino << " | ";
  cout << uppercase;
  cout << oct
       << setw(15) << left  << "mode"
       << setw(4)  << right << "o" << setfill('0') 
       << setw(6)  << right << inode.mode  << setfill(' ')
       << dec << endl;

  cout << setw(15) << left << "uid" 
       << setw(10) << right << inode.uid << " | ";
  cout << setw(15) << left << "size" 
       << setw(10) << right << inode.size << endl;

  cout << setw(15) << left << "gid" 
       << setw(10) << right << inode.gid << " | ";
  cout << setw(15) << left << "link count" 
       << setw(10) << right << inode.link_cnt << endl;

  cout << setw(15) << left << "blocks" 
       << setw(10) << right << inode.blocks << " | ";
  cout << hex
       << setw(15) << left << "flags"
       << setfill('0') << "0x" 
       << setw(8) << right << inode.flags
       << setfill(' ') << dec << endl;

  for(int i = 0 ; i < 15 ; i++) {
    if(!(i % 2)) cout << endl;
    else cout << " | ";
    cout << "i_block[" << setw(2) << i << "]" 
	 << setw(4) << ""
	 << hex 
	 << setfill('0')
	 << setw(2) << "0x" 
	 << setw(8) << right << inode.block[i]
	 << setfill(' ')
	 << dec;
  }

  cout << setfill(' ');
  cout << endl << endl;

  if(m_dump) {
    cout << "i-node hex code" << endl;
    _dump((_byte*)&inode, sizeof(inode));
    cout << endl;
  }

  // Check whether or not symbolic link file.
  if(S_ISLNK(inode.mode)) {
    _print_symbolic_link(inode);
  }
  else if(S_ISDIR(inode.mode)) {
    for(int i = 0 ; i < inode.blocks ; i++)
      if(inode.block[i] != 0) 
	_print_dir_entries(inode.block[i]);
  }
  else if(S_ISREG(inode.mode)) 
    _print_regfile_block(inode);
}

void Ext2p::print_inode(_dword ino)
{
  i_node inode = m_ext->get_inode(ino);
  _print_inode(ino, inode);
}

void Ext2p::_dump(const _byte* buf, size_t size)
{
  int col = 16;
  int line = size / col;

  // Display dump title
  cout << uppercase;
  cout << setw(5) << setfill(' ') << " ";
  for(int i = 0 ; i < col ; i++)
    cout << setw(2) << setfill('0') << dec << i << " ";
  cout << setw(13) << setfill(' ') << "ASCII code" << endl; 
  cout << setw(68) << setfill('-') << "" <<endl;

  cout << hex;

  for(int i = 0 ; i < line ; i++) {
    int offset = i * col;
    
    cout << setw(3) << setfill('0') << hex << offset << dec << "  ";

    // Hex value
    for(int j = 0 ; j < col ; j++) 
      cout << setw(2) << setfill('0') << hex << (_dword)(buf[offset + j]) << dec  << " ";
          
    // ASCII code
    for(int j = 0 ; j < col ; j++) {
      char ch = buf[offset + j];
      if(ch > 31 && ch < 127) cout << setw(1) << (char)(buf[offset + j]);
      else cout << setw(1) << ".";
    }
    cout << endl;      
  }
  cout << setfill(' ');
  
}

void Ext2p::dump_block(_dword num)
{
  _byte* buf = 0;
  _dword block_size = m_ext->get_block_size();
  
  buf = new _byte[block_size];
  if(!buf) return;

  // Search out block from file system.
  m_ext->get_block(num, buf, block_size);

  cout << "Block # " << num << endl;
  _dump(buf, block_size);

  delete[] buf;
}

void Ext2p::print_inode_map(_dword group_num)
{
  _byte* buf = 0;
  _dword block_size = m_ext->get_block_size();

  buf = new _byte[block_size];
  if(!buf) return;

  // Get inode bitmap by group number.
  int retval = m_ext->get_inode_bitmap(group_num, buf, block_size);

  _dump(buf, retval);

  delete[] buf;
}

void Ext2p::print_block_map(_dword group_num)
{
  _byte* buf = 0;
  _dword block_size = m_ext->get_block_size();

  buf = new _byte[block_size];
  if(!buf) return;

  int retval = m_ext->get_block_bitmap(group_num, buf, block_size);

  _dump(buf, retval);

  delete[] buf;
}

void Ext2p::_print_dir_entries(_dword block_num)
{
  size_t size = m_ext->get_block_size();
  _byte* buf = new _byte[size];
  vector<ext2_dir_entry> entries;

  if(!buf) return ;
  
  m_ext->get_block(block_num, buf, size);
 
  if(m_dump) {
    cout << right << "Block #" 
	 << setw(2) << " " 
	 << block_num << " "
	 << "[0x" << hex << block_num << "]" << dec << endl; 
    cout << "Directory block hex code" << endl;
    _dump(buf, size);
    cout << endl;
  }

  int ret = m_ext->get_dir_entries(buf, size, entries);
  
  cout << setw(10) << right << "Block #" 
       << setw(2) << " " 
       << block_num << " "
       << "[0x" << hex << block_num << "]" << dec << endl;
  
  cout << setw(10) << right << "" << setw(2) << " "
       << setw(20) << left << "" 
       << setw(5) << right << ""
       << setw(7) << right << "name"
       << setw(7) << right << "record" << endl;

  cout << setw(10) << right << "i-node #" << setw(2) << " "
       << setw(20) << left << "file name" 
       << setw(5) << right << "type"
       << setw(7) << right << "length"
       << setw(7) << right << "length" << endl;

  for(int i = 0 ; i < ret ; i++) {
    cout << setw(10) << right << entries[i].inode << setw(2) << " "
	 << setw(20) << left << entries[i].name 
	 << setw(5) << right << hex << (_word) entries[i].file_type << dec 
	 << setw(7) << right << (_word)entries[i].name_len
	 << setw(7) << right << entries[i].rec_len << endl;
  }

  delete[] buf;
}

void Ext2p::_print_regfile_block(const i_node& inode)
{
  size_t size = m_ext->get_block_size();
  _byte* buf = new _byte[size + 1];
  buf[size] = 0;
 
  for(int i = 0 ; i < 12 ; i++) {
    if(inode.block[i]) {
      memset(buf, 0, size);
      m_ext->get_block(inode.block[i], buf, size);
      _dump(buf, size);
      cout << buf;
    }
  }

  cout << endl;
  delete[] buf;
}
