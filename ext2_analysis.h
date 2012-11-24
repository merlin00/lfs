#ifndef _EXT2_ANALYSIS_H_
#define _EXT2_ANALYSIS_H_

#include <vector>
#include "extX.h"

class ExtAnalysis
{
 private:
  int m_fd;
  ext_super_block m_super_block;
  std::vector<group_desc> m_group_descs;

  int _read_super_block();
  int _read_group_descs(const ext_super_block& super);

 public:

  ExtAnalysis();
  ~ExtAnalysis() {}
  ExtAnalysis(const char* filename);

  bool open(const char* filename);
  void close();

  const ext_super_block& get_super_block() { 
    return m_super_block;
  }
  const std::vector<group_desc>& get_group_desc() {
    return m_group_descs;
  }

  int get_inode_bitmap(_dword group_num, 
			_byte* bitmap,
			size_t size);
  int get_block_bitmap(_dword group_num,
		       _byte* bitmap,
		       size_t size);

};


#endif
