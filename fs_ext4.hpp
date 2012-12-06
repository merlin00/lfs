/***************************************************************/
/*
  Copyright (C) 2012 Kwon kideok (merlin00@pusan.ac.kr)
  Ubiqutious Computing Laboratory in Pusan National University

  2012.12.03  

 */
/***************************************************************/
#ifndef _FS_EXT4_H_
#define _FS_EXT4_H_

// FsExt4 class

class FsExt4 : public FsExt2 
{
public:
  FsExt4() {}
  FsExt4(const char* filename) : FsExt2(filename) {}
  ~FsExt4() {}

  ext4_extent_header get_block_tree(const char* block, 
				    size_t size, 
				    std::vector<ext4_extent>& entries);
};

#endif
