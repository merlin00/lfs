/***************************************************************/
/*
  Copyright (C) 2012 Kwon kideok (merlin00@pusan.ac.kr)
  Ubiqutious Computing Laboratory in Pusan National University

  2012.12.03  

 */
/**************************************************************/
#include <iostream>
#include <vector>

#include <string.h>

#include "ext2.h"
#include "ext4.h"
#include "fs_ext2.hpp"
#include "fs_ext4.hpp"

using namespace std;

ext4_extent_header FsExt4::get_block_tree(const char* block, 
					   size_t size, 
					   vector<ext4_extent>& entries)
{
  size_t offset = 0;
  size_t total;
  int i;
  ext4_extent_header hdr;
  
  memcpy(&hdr, block, sizeof(ext4_extent_header));
  offset += sizeof(ext4_extent_header);

  if(hdr.eh_magic != EXTENT_MAGIC) return hdr;
  total = hdr.eh_entries * sizeof(ext4_extent) + sizeof(ext4_extent_header);
  if(total > size) return hdr;

  for(i = 0 ; i < hdr.eh_entries ; i++) {
    ext4_extent entry;

    memset(&entry, 0, sizeof(ext4_extent));
    memcpy(&entry, block + offset, sizeof(ext4_extent));

    entries.push_back(entry);

    offset += sizeof(ext4_extent);    
  }

  return hdr;
}
