#include <iostream>
#include <vector>
#include <stdio.h>

#include "extX.h"
#include "ext2analysis.hpp"
#include "ext2presentation.hpp"

using namespace std;

Ext2Presentation::Ext2Presentation() 
  : m_ext2(0)
{
}

Ext2Presentation::~Ext2Presentation()
{
}

void Ext2Presentation::print_super_block()
{
  if(!m_ext2) return;
  
  const ext_super_block& sb = m_ext2->get_super_block();
  
  printf("%-22s : %10s \r\n", "Last mounted on", sb.last_mounted);
  printf("%-22s : %10u \r\n", "Inode count", sb.inode_cnt);
  printf("%-22s : %10u \r\n", "Block count", sb.blk_cnt);	
  printf("%-22s : %10u \r\n", "Res block count", sb.res_blk_cnt);	
  printf("%-22s : %10u \r\n", "Free block count", sb.free_blk_cnt);
  printf("%-22s : %10u \r\n", "Free inode count", sb.free_inode_cnt);
  printf("%-22s : %10u \r\n", "First data node block", sb.first_data_blk);
  printf("%-22s : %10u \r\n", "Block size", GET_BLOCK_SIZE(sb.log_blk_size));	
  printf("%-22s : %10u \r\n", "Fragment size", GET_BLOCK_SIZE(sb.log_frag_size));
  printf("%-22s : %10u \r\n", "Blocks per group", sb.blks_per_group);	
  printf("%-22s : %10u \r\n", "Fragments per group", sb.frags_per_group);	
  printf("%-22s : %10u \r\n", "Inode per group", sb.inode_per_group);
  printf("%-22s : %10u \r\n", "Mount count", sb.mount_cnt);	
  printf("%-22s : %10u \r\n", "Max mount count", (_dword)sb.max_mount_cnt);	
  printf("%-22s : %6s%4X \r\n", "Magic", "0x", sb.magic);
  printf("%-22s : %10u \r\n", "Block group #", (_dword)sb.blk_group_num);	
  printf("%-22s : %10u \r\n", "Inode size", sb.inode_size);	
  printf("\r\n");
}


void Ext2Presentation::print_element_of_group_desc(const vector<group_desc>& groups)
{
  // Print group descriptor
  size_t size = groups.size();

  for(size_t i = 0 ; i < size ; i++) {
    printf("%3d  %9u   %9u   %9u   %7u   %7u   %7u \r\n",
	   i, 
	   groups[i].blk_bitmap, 
	   groups[i].inode_bitmap, 
	   groups[i].inode_table,
	   groups[i].free_blk_cnt, 
	   groups[i].free_inode_cnt, 
	   groups[i].useddir_cnt);
  }
}

void Ext2Presentation::print_title_of_group_desc() 
{
  // Print titile
  printf("%s\r\n", "======================================================================");
  printf("%3s  %10s  %10s  %10s  %8s  %8s  %8s\r\n",
	 "no", "blk map", "ino map", "ino table",
	 "free blk", "free ino", "used dir");
  printf("%s\r\n", "----------------------------------------------------------------------");
}

void Ext2Presentation::print_group_desc()
{
  print_title_of_group_desc();
  print_element_of_group_desc(m_ext2->get_group_desc());
}

void Ext2Presentation::print_inodes_in_group(_dword group_num)
{
  Ext2Analysis::vect_inodes inodes;

  m_ext2->get_inodes(group_num, inodes);  
  _print_inodes_in_group(inodes);
}

void Ext2Presentation::_print_inodes_in_group(const Ext2Analysis::vect_inodes &inodes)
{
  // Print group descriptor
  size_t size = inodes.size();

  for(size_t i = 0 ; i < size ; i++) {
    const Ext2Analysis::pair_inode& inode = inodes[i];
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

void Ext2Presentation::_print_inode(_dword ino, const i_node& inode)
{
  printf("%-15s : %10d \r\n", "inode #", ino);
  printf("%-15s : %6s%04X \r\n", "mode", "", inode.mode);
  printf("%-15s : %6s%04X \r\n", "uid", "",inode.uid);
  printf("%-15s : %10u \r\n", "size", inode.size);
  printf("%-15s : %6s%04x \r\n", "gid", "", inode.gid);
  printf("%-15s : %10u \r\n", "link count", inode.link_cnt);
  printf("%-15s : %10u \r\n", "blocks", inode.blocks);
}

void Ext2Presentation::print_inode(_dword ino)
{
  i_node inode = m_ext2->get_inode(ino);
  _print_inode(ino, inode);
}
