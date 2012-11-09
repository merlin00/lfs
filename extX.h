#ifndef _EXTX_H_
#define _EXTX_H_

typedef unsigned short _word;
typedef unsigned char _byte;

typedef enum _block_type{
  _0k = 0,
  _1k = 1024,
  _2k = 2046,
  _4k = 4096
}block_type;

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

void attach();

int create_block(block_type type, block_des* blk);
int read_block(int fd, block_des* blk);
void delete_block(block_des* blk);

int get_direct_img(int fd, dirent_disk_img* pdir);

#endif
