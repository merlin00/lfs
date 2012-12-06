/***************************************************************/
/*
  Copyright (C) 2012 Kwon kideok (merlin00@pusan.ac.kr)
  Ubiqutious Computing Laboratory in Pusan National University

  2012.12.03  

 */
/***************************************************************/
#ifndef _EXT4P_HPP_
#define _EXT4P_HPP_
class Ext4p : public Ext2p
{
protected:
  void _print_extent_header(const ext4_extent_header& hdr);
  void _print_extent(const char* block, size_t size);
  void _print_extent_idx(const ext4_extent& entry);
  void _print_extent_leaf(const ext4_extent& entry);

public:
  Ext4p() {}
  ~Ext4p() {}
};
#endif
