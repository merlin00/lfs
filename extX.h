#ifndef _EXTX_H_
#define _EXTX_H_

#define SIZE_OF_BLOCK 4096
#define MAGIC_OFFSET_OF_SUPERBLOCK 0x38

#define INODES_PER_GROUP 8192

typedef unsigned short _word;
typedef unsigned int _dword;
typedef unsigned char _byte;

typedef enum _block_type{
  _1K = 1024,
  _2K = 2046,
  _4K = 4096
}block_type;

typedef struct _extX_info {
  unsigned int blks_of_group;	 // Blocks of group
  unsigned int inodes_per_group; // i-nodes per group
  unsigned short size_of_blk;	 // Size of block
  unsigned short size_of_bitmap; // Bitmap size 
}extX_info;

// File blcok descriptor
typedef struct _block_des {
  size_t size;
  char *block;
}block_des;

// Disk image structure
#define HDR_SIZE_OF_DIRENT 8

typedef struct _dirent_disk_img{
  __ino_t d_ino;		// I-node number
  _word rec_len;		// Record length
  _byte name_len; 		// Name length
  _byte typ;			// File Type
  char* name;
}dirent_disk_img;

/* Superblock magic number 0xEF53 */
#define MAGIC_NUM_OF_SUPERBLOCK 0xEF53
#define SIZE_OF_SUPERBLOCK 264

/* Linux superblock */
typedef struct _fs_superblock {
  _dword inode_cnt;	        /* Inode count */
  _dword blk_cnt;		/* block count */
  _dword res_blk_cnt;		/* res block count */
  _dword free_blk_cnt;		/* Free block count */
  _dword free_inode_cnt;	/* Free inode count */
  _dword first_data_blk;	/* First data node block */
  _dword log_blk_size;		/* Log block size */
  _dword log_frag_size;		/* Log frag size */
  _dword blk_per_grp;		/* Block per group */
  _dword frag_per_grp;		/* Frag per group */
  _dword inode_per_grp;		/* Inode per group */
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
  _word blk_grp_num;		/* Block group number */
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
}fs_superblock;

/* Group descriptor */

#define SIZE_OF_GROUP_DESCRIPTOR 32 /* 32 bytes */
typedef struct _group_descriptor {
  _dword blk_bitmap;		/* Block bitmap */
  _dword inode_bitmap;		/* Inode bitmap */
  _dword inode_table;		/* Inode table */
  _word free_blk_cnt;		/* Free block count */
  _word free_inode_cnt;		/* Free inode count */
  _word useddir_cnt;		/* Used directory count */
  _word padding1;
  _dword padding2;
  _dword padding3;
  _dword padding4;
}grp_des;

/* Inode */
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
  _dword osdl;
  _dword block[15];
  _dword generation;
  _dword file_acl;
  _dword dir_acl;
  _dword faddr;
  _byte osd2[12];
}i_node;

#define BLOCK_GROUP_OF_INODE(inode, inodes_per_group) \
  (inode - 1) / inodes_per_group
#define ENTRY_INODE_TABLE(inode, inodes_per_group) \
  (inode - 1) % inodes_per_group

int get_superblock_from_fs(int fd, fs_superblock* superblk);
std::vector<grp_des>& get_grp_des_from_fs(int fd, block_type type);

#endif
