/***************************************************************/
/*
  Copyright (C) 2012  Kwon kideok (merlin00@pusan.ac.kr)
  Ubiqutious Computing Laboratory in Pusan National University

  2012.12.03  

 */
/***************************************************************/

#ifndef _EXT2_H_
#define _EXT2_H_

#define _1K_BLOCK 1024
#define _2K_BLOCK 2048
#define _4K_BLOCK 4096

#define SIZE_OF_BLOCK 4096
#define MAGIC_OFFSET_OF_SUPERBLOCK 0x38

#define INODES_PER_GROUP 8192

typedef unsigned short _word;
typedef unsigned int _dword;
typedef unsigned char _byte;

typedef struct _extX_info {
  unsigned int blks_of_group;	 // Blocks of group
  unsigned int inodes_per_group; // i-nodes per group
  unsigned short size_of_blk;	 // Size of block
  unsigned short size_of_bitmap; // Bitmap size 
}extX_info;

/* Superblock magic number 0xEF53 */
#define MAGIC_NUM_OF_SUPERBLOCK 0xEF53
#define POS_OF_SUPERBLOCK 1024
#define SIZE_OF_SUPERBLOCK 264

#define GET_BLOCK_SIZE(X) ((1 << X) * _1K_BLOCK)

// Linux ext superblock 
typedef struct _fs_super_block {
  _dword inode_cnt;	        /* Inode count */
  _dword blk_cnt;		/* block count */
  _dword res_blk_cnt;		/* res block count */
  _dword free_blk_cnt;		/* Free block count */
  _dword free_inode_cnt;	/* Free inode count */
  _dword first_data_blk;	/* First data node block */
  _dword log_blk_size;		/* Log block size */
  _dword log_frag_size;		/* Log frag size */
  _dword blks_per_group;	/* Block per group */
  _dword frags_per_group;	/* Frag per group */
  _dword inode_per_group;	/* Inode per group */
  _dword mtime;			/* mtime */
  _dword wtime;			/* wtime */
  _word mount_cnt;		/* Mount count */ 
  _word max_mount_cnt;		/* Max mount count */
  _word magic;			/* Magic */
  _word state;			/* State */
  _word err;			/* Error */
  _word minor;			/* Minor version */
  _dword last_chk;		/* Last check */
  _dword chk_interval;		/* Check interval */
  _dword creator_os;		/* Creator OS */
  _dword major;			/* Major version */
  _word def_res_uid;		/* def_res uid */
  _word def_res_gid;		/* def_res gid */
  _dword first_ino;		/* First ino */
  _word inode_size;		/* Inode size */
  _word blk_group_num;		/* Block group number */
  _dword compat;		/* Feature compat */
  _dword incompat;		/* Feature incompat */
  _dword read_only_compat;	/* feature read only compat */
  _byte uuid[16];		/* UUID [16 bytes] */
  _byte valume_name[16];	/* Valume Name [16 bytes] */
  _byte last_mounted[64];	/* Last mounted [64 bytes] */
  _dword bitmap;		/* Algorithm usage bitmap */
  _byte prealloc_blk;		/* prealloc block */
  _byte prealloc_dir;           /* prealloc dir block */
  _word padding1;		/* Padding1 */
  _byte journal_uuid[16];	/* Journal uuid */
  _dword journal_inode_num;	/* Jouranl inode number */
  _dword journal_dev;		/* Journal dev */
  _dword last_orphan;		/*  */
  _byte hash_seed[16];		/* Hash seed */
  _byte hash_version;		/* Hash version and padding */
  _byte padding2;		/* Padding2 */
  _word padding3;		/* Padding3 */
  _dword default_mount_opt;	/* Default mount option */
  _dword first_meta_bg;		/* First meta block group */
}fs_super_block, ext_super_block;

// Group descriptor
#define SIZE_OF_GROUP_DESCRIPTOR 32 /* 32 bytes */

typedef struct _group_descriptor {
  _dword block_bitmap;		/* Block bitmap */
  _dword inode_bitmap;		/* Inode bitmap */
  _dword inode_table;		/* Inode table */
  _word free_blk_cnt;		/* Free block count */
  _word free_inode_cnt;		/* Free inode count */
  _word useddir_cnt;		/* Used directory count */
  _word padding1;
  _dword padding2;
  _dword padding3;
  _dword padding4;
}group_desc;

/* Inode */
#define SIZE_OF_INODE 128	/* 128 bytes */

typedef struct _i_node {
  _word mode;
  _word uid;
  _dword size;
  _dword atime;
  _dword ctime;
  _dword mtime;
  _dword dtime;
  _word gid;
  _word link_cnt;
  _dword blocks;
  _dword flags;  
  _dword osd1;
  _dword block[15];
  _dword generation;
  _dword file_acl;
  _dword dir_acl;
  _dword faddr;
  _byte osd2[12];
}i_node;

#endif
