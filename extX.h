#ifndef _EXTX_H_

#define _EXTX_H_

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

// Disk image structure
#define HDR_SIZE_OF_DIRENT 8

typedef struct _dirent_disk_img{
  __ino_t d_ino;		// I-node number
  _word rec_len;		// Record length
  _byte name_len; 		// Name length
  _byte typ;			// File Type
  char* name;
}dirent_disk_img;


#define HDR_SIZE_OF_DIR_ENTRY 8
typedef struct _ext2_dir_entry {
  __ino_t ino;
  _word rec_len;
  _byte name_len;
  _byte file_type;
  char name[256];
}ext2_dir_entry;

/* Superblock magic number 0xEF53 */
#define MAGIC_NUM_OF_SUPERBLOCK 0xEF53
#define POS_OF_SUPERBLOCK 1024
#define SIZE_OF_SUPERBLOCK 264

#define GET_BLOCK_SIZE(X) ((1 << X) * _1K_BLOCK)

/* Linux superblock */
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

/* Group descriptor */
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


/*
 * Inode flags
 */
#define	EXT4_SECRM_FL			0x00000001 /* Secure deletion */
#define	EXT4_UNRM_FL			0x00000002 /* Undelete */
#define	EXT4_COMPR_FL			0x00000004 /* Compress file */
#define EXT4_SYNC_FL			0x00000008 /* Synchronous updates */
#define EXT4_IMMUTABLE_FL		0x00000010 /* Immutable file */
#define EXT4_APPEND_FL			0x00000020 /* writes to file may only append */
#define EXT4_NODUMP_FL			0x00000040 /* do not dump file */
#define EXT4_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#define EXT4_DIRTY_FL			0x00000100
#define EXT4_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#define EXT4_NOCOMPR_FL			0x00000400 /* Don't compress */
#define EXT4_ECOMPR_FL			0x00000800 /* Compression error */
/* End compression flags --- maybe not all used */
#define EXT4_INDEX_FL			0x00001000 /* hash-indexed directory */
#define EXT4_IMAGIC_FL			0x00002000 /* AFS directory */
#define EXT4_JOURNAL_DATA_FL		0x00004000 /* file data should be journaled */
#define EXT4_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#define EXT4_DIRSYNC_FL			0x00010000 /* dirsync behaviour (directories only) */
#define EXT4_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#define EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#define EXT4_EXTENTS_FL			0x00080000 /* Inode uses extents */
#define EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
#define EXT4_EOFBLOCKS_FL		0x00400000 /* Blocks allocated beyond EOF */
#define EXT4_RESERVED_FL		0x80000000 /* reserved for ext4 lib */

#define EXT4_FL_USER_VISIBLE		0x004BDFFF /* User visible flags */
#define EXT4_FL_USER_MODIFIABLE		0x004B80FF /* User modifiable flags */

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

typedef struct _ext4_extent_header {
  _word eh_magic;
  _word eh_entries;
  _word eh_max;
  _word eh_depth;
  _dword eh_generation;
}ext4_extent_header;

typedef union _ext4_extent {
  typedef struct _ext4_extent_idx {
    _dword ei_block;
    _dword ei_leaf_lo;
    _word ei_leaf_hi;
    _word ei_unused;
  }ext4_extent_idx;

  typedef struct _ext4_extent_leaf {
    _dword ee_block;
    _dword ee_len;
    _dword ee_start_hi;
    _dword ee_start_lo;
  }ext4_extent_leaf;
}ext4_extent;

typedef struct _ext4_extent_tail {
  _dword eb_checksum;
}ext4_extent_tail;

#define BLOCK_GROUP_OF_INODE(inode, inodes_per_group) \
  (inode - 1) / inodes_per_group
#define ENTRY_INODE_TABLE(inode, inodes_per_group) \
  (inode - 1) % inodes_per_group

#endif
