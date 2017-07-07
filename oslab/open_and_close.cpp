#include <stdio.h>
#include "FILESYS.h"
#include "open_and_close.h"
#include"search_and_dir.h"
#include"igetput.h"
#include"ballocfre.h"
#include"access.h"

unsigned short open_file(int user_id, char *filename, unsigned short openmode)
{
	unsigned int dinodeid;
	struct inode * inode;
	int i, j;
	dinodeid = namei(filename);
	if (dinodeid == 0)    /* nosuchfile */
	{
		printf("\nfile does not exist!\n");
		return 0;
	}
	inode = iget(dinodeid);
	if (!access(user_id, inode, openmode))    /* access denied */
	{
		printf("\nfile open has not access!\n");
		iput(inode);
		return 0;
	}
	/* alloc the sys-ofile item */
	for (i = 1; i<SYSOPENFILE; i++)
		if (sys_ofile[i].f_count == 0) break;
	if (i == SYSOPENFILE)
	{
		printf("\nsystem open file too much\n");
		iput(inode);
		return 0;
	}
	sys_ofile[i].f_inode = inode;
	sys_ofile[i].f_flag = openmode;
	sys_ofile[i].f_count = 1;
	if (openmode & FAPPEND)
		sys_ofile[i].f_off = inode->di_size;
	else
		sys_ofile[i].f_off = 0;
	/* alloc the user open file item */
	for (j = 0; j<NOFILE; j++)
		if (user[user_id].u_ofile[j] == SYSOPENFILE+1) break;
	if (j == NOFILE)
	{
		printf("\nuser open file too much!\n");
		sys_ofile[i].f_count = 0;
		iput(inode);
		return 0;
	}
	user[user_id].u_ofile[j] = 1;
	/*if APPEND, free the block of the file before */
	if (!(openmode & FAPPEND))
	{
		for (i = 0; i<inode->di_size / BLOCKSIZ + 1; i++)
			bfree(inode->di_addr[i]);
		inode->di_size = 0;
	}
	return j;
}
void close_file(unsigned int user_id, unsigned short cfd)
{
	struct inode *inode;
	inode = sys_ofile[user[user_id].u_ofile[cfd]].f_inode;
	iput(inode);
	sys_ofile[user[user_id].u_ofile[cfd]].f_count--;
	user[user_id].u_ofile[cfd] = SYSOPENFILE + 1;
}