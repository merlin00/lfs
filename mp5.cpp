#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <math.h>

#include "ext2.h"
#include "fs_ext2.hpp"
#include "ext2p.hpp"

using namespace std;

void print_select_menu()
{
  printf("%s\r\n", "=======================================");
  printf("%s\r\n", "0. Exit");
  printf("%s\r\n", "1. Open file or device");
  printf("%s\r\n", "2. Calucation on block group #");
  printf("%s\r\n", "3. Super block information");
  printf("%s\r\n", "4. Group descriptor table");
  printf("%s\r\n", "5. Inode info");
  printf("%s\r\n", "6. Inode table in group #");
  printf("%s\r\n", "7. Dump a block");
  printf("%s\r\n", "8. Block bitmap");
  printf("%s\r\n", "9. Inode bitmap");
  printf("%s\r\n", "=======================================");
}

int input_num(const char* title)
{
  int num = 0;

  printf("%s >", title);
  scanf("%d", &num);

  return num;
}

void calculate_block_group()
{
  int num_of_inode = 0;
  int num_of_group =0;
  int entry =0;

  printf("%s\r\n", "***************************************");
  printf("\t %s\r\n", "Calucation on block group");
  printf("%s\r\n", "***************************************");

  printf("%-15s > ", "Input I-Node #");
  scanf("%d", &num_of_inode);
  printf("\r\n");

  //num_of_group = BLOCK_GROUP_OF_INODE(num_of_inode, INODES_PER_GROUP);
  //  entry = ENTRY_INODE_TABLE(num_of_inode, INODES_PER_GROUP);

  printf("%s\r\n", "---------------------------------------");
  printf("%-27s : %d\r\n", "i-node num", num_of_inode);
  printf("%-27s : %d\r\n", "Block group number", num_of_group);
  printf("%-27s : %d\r\n", "The offset of i-node table", entry);
  printf("%s\r\n", "---------------------------------------");
  printf("%s", "Press any key");

  getchar();
  while(getchar() != '\n');
  printf("\r\n");
}

int open_file_from_input(char* sz_title)
{
  char filename[256];
  int fd = 0;
  fs_super_block superblk;

  printf("%s\r\n", "***************************************");
  printf("\t %s\r\n", sz_title);
  printf("%s\r\n", "***************************************");
  
  // printf("%-12s ./> ", "Filename");
  // scanf("%s", filename);
  strcpy(filename, "/dev/mapper/vg_kkd-lv_root");
  printf("%-12s ./> %s\r\n", "Filename", filename);

  fd = open(filename, O_RDONLY);
  if(fd < 0) {
    printf("Worng filename!!!\r\n");
    return -1;
  }
  printf("%s ... OK\r\n", "File opened");

  return fd;
}

#define SIZE_OF_CMD_OPT 10
#define SIZE_OF_OPT_PARAM 256

typedef struct _cmd_option {
  char opt[SIZE_OF_CMD_OPT];
  char param[SIZE_OF_OPT_PARAM];
}cmd_option;

int parse_cmd_opt(char* opt, size_t size, cmd_option& cmd_opt)
{
  int i = 0, j;
  
  for(j = 0 ; i < size && opt[i] != '=' ; i ++)
    cmd_opt.opt[j++] = opt[i];
  cmd_opt.opt[j] = 0;

  if(!(i < size)) return -1;
  i++;
  for(j = 0 ; i < size ; i++)
    cmd_opt.param[j++] = opt[i];
  cmd_opt.param[j] = 0;

  return (opt[0] == '-') ? 0 : 1;  
}

// Print option
/* 
   -s   print super block
   -gt  print group descriptor table
   -it  print inode table
   dump print hex code
   -imap print inode bitmap
       gno=group number
   -bmap print block bitmap
       gno=group number
   -inode print inode
       ino=inode number
   -group print group descriptor
       gno=group number
   -block print block dump
       bno=block number
*/
void interactive_option(const Ext2p& present)
{
  int sel = 5;
  /*
  do {
    print_select_menu();
    sel = input_num("?");

    switch(sel) {
      case 2: calculate_block_group(); break;
      case 5: present.print_inode(input_num("inode #")); break;
      case 7: present.dump_block(input_num("Block #")); break;
      case 8: present.print_block_map(input_num("Group #")); break;
      case 9: present.print_inode_map(input_num("Group #")); break;
    default:  break;
    }
    }while(sel);*/
  

}


int main(int argc, char* argv[])
{
  //  int sel = 5;

  _dword gno, ino, bno;
  vector<char*> param_opts, exec_opts;

  FsExt2 ext;
  Ext2p present;

  gno = ino = bno = 0;

  for(int i = 1 ; i < argc ; i++)
    if(argv[i][0] == '-') exec_opts.push_back(argv[i]);
    else param_opts.push_back(argv[i]);

  while(param_opts.size()) {
    char* opt = param_opts.back();
    cmd_option cmd_opt;

    param_opts.pop_back();
    memset(&cmd_opt, 0, sizeof(cmd_option));

    parse_cmd_opt(opt, strlen(opt), cmd_opt);

    if(strcmp(cmd_opt.opt, "if") == 0) {
      cout << cmd_opt.param << " open ... ";
      if(!ext.open(cmd_opt.param)) {
	cout << "failed " << endl;
	return -1;
      }
      cout << "ok" << endl; 
    }
    else if(!strcmp(cmd_opt.opt, "gno")) { gno = atol(cmd_opt.param); cout << "gno=" << gno << endl; }
    else if(!strcmp(cmd_opt.opt, "ino")) {
      ino = atol(cmd_opt.param);
    }
    else if(!strcmp(cmd_opt.opt, "bno")) bno = atol(cmd_opt.param);
    else if(!strcmp(cmd_opt.opt, "dump")){
      if(!strcmp(cmd_opt.param, "true")) present.set_dump();
      else present.clear_dump();
    }

  }

  present.attach(&ext);

  while(exec_opts.size()) {
    char* opt = exec_opts.back();
    exec_opts.pop_back();
    
    if(!strcmp(opt, "-s")) present.print_super_block();         // Print super block;
    else if(!strcmp(opt, "-gt")) present.print_group_desc();    // Print group descriptor table
    else if(!strcmp(opt, "-bmap")) present.print_block_map(gno);   // Print block bitmap
    else if(!strcmp(opt, "-imap")) present.print_inode_map(gno);   // Print inode bitmap
    else if(!strcmp(opt, "-it")) present.print_inodes_in_group(gno);   // Print inode table
    // else if(strcmp(opt, "-group") == 0) present.print_super_block();   // Print group descriptor
    else if(!strcmp(opt, "-inode")) present.print_inode(ino);   // Print inode
    else if(!strcmp(opt, "-block")) present.dump_block(bno);   // Dump  block
    
  }

  /*
 

  */
  return 0;
}
