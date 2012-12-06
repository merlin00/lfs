/***************************************************************/
/*
  Copyright (C) 2012 Kwon kideok (merlin00@pusan.ac.kr)
  Ubiqutious Computing Laboratory in Pusan National University

  2012.12.03  

 */
/***************************************************************/
#include <iostream>
#include <vector>

#include "ext2.h"
#include "ext4.h"
#include "fs_ext2.hpp"
#include "fs_ext4.hpp"
#include "ext2p.hpp"
#include "ext4p.hpp"

void Ext4Presentation::_print_extent(const char* block, size_t size)
{
  ext4_extent_header hdr;
  vector<ext4_extent> entries;

  memset(&hdr, 0, sizeof(ext4_extent_header));
  hdr = m_ext->get_block_tree(block, size, entries);
  
  _print_extent_header(hdr);

  for(int i = 0 ; i < hdr.eh_entries ; i++) {
    _print_extent_leaf(entries[i]);
  }
}

void Ext4p::_print_extent_header(const ext4_extent_header& hdr)
{
  cout << setw(15) << left << "Extent header" << endl;
  cout << setw(15) << left << "\tMagic" << "0x" << hex << hdr.eh_magic << dec << endl;
  cout << setw(15) << left << "\tEntries" << hdr.eh_entries << endl;;
  cout << setw(15) << left << "\tMax" << hdr.eh_max << endl;
  cout << setw(15) << left << "\tDepth" << hdr.eh_depth << endl;
  cout << setw(15) << left << "\tGeneration" << hdr.eh_generation << endl;
}

void Ext4p::_print_extent_idx(const ext4_extent& entry)
{
}

void Ext4p::_print_extent_leaf(const ext4_extent& entry)
{
  cout << setw(15) << left << "Extent leaf" << endl;
  cout << setw(15) << left << "\tBlock" << entry.leaf.ee_block << endl;
  cout << setw(15) << left << "\tLength" << entry.leaf.ee_len << endl;;
  cout << setw(15) << left << "\tStart Hi" << entry.leaf.ee_start_hi << endl;
  cout << setw(15) << left << "\tStart Lo" << entry.leaf.ee_start_lo << endl;
}
