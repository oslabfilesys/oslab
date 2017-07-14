#include <stdio.h>
#include "FILESYS.h"
#include "open_and_close.h"
#include"search_and_dir.h"
#include"igetput.h"
#include"ballocfre.h"
#include"access.h"

unsigned short open_file(int user_id, const char *filename, unsigned short openmode)//用户ID；文件名；打开权限
{
	unsigned int dinodeid;
	struct inode * inode;
	int i, j;
	dinodeid = search_directory_or_file_id_by_name ( directory, filename);
	if (dinodeid == NOTFOUND )    /* nosuchfile */
	{
		printf("\nfile does not exist!\n");
		return OPEN_FAILED;
	}
	inode = iget(dinodeid);
	if (!access(user_id, inode, openmode))    /* access denied */
	{
		printf("\nfile open has not access!\n");
		iput(inode);
		return OPEN_FAILED;
	}
	/* alloc the sys-ofile item */
	for (i = 1; i<SYSOPENFILE; i++)
		if (sys_ofile[i].f_count == 0) break;
	if (i == SYSOPENFILE)//文件打开数量
	{
		printf("\nsystem open file too much\n");
		iput(inode);
		return OPEN_FAILED;
	}

	sys_ofile[i].f_inode = inode;
	sys_ofile[i].f_flag = openmode;
	sys_ofile[i].f_count = 1;
/*	if (openmode & ADD_AB)
		sys_ofile[i].f_off = inode->di_size;
	else
		sys_ofile[i].f_off = 0;
	/* alloc the user open file item */
	for (j = 0; j<NOFILE; j++)
		if ( users [user_id].u_ofile[j] == SYSOPENFILE+1) break;
	if (j == NOFILE)
	{
		printf("\nuser open file too much!\n");
		sys_ofile[i].f_count = 0;
		iput(inode);
		return OPEN_FAILED;
	}
    users [user_id].u_ofile[j] = i;
    //如果读文件的话

    if ( openmode == READ_AB )
    {
        sys_ofile [i].f_off = 0;
    }
    //如果写文件的话
	else if (openmode == WRITE_AB)
	{
        sys_ofile [i].f_off = 0;
		for (i = 0; i<inode->di_size / BLOCKSIZ + 1; i++)
			bfree(inode->di_addr[i]);
        inode->di_addr [0] = balloc ( );
		inode->di_size = 0;
	}
    //如果在后面添加的话
    else if ( openmode == ADD_AB )
    {
        sys_ofile [i].f_off = inode->di_size;
    }
	return j;
}
void close_file ( unsigned int user_id, unsigned short cfd )//u_ofile 用户打开文件表
{
    struct inode *inode;
    inode = sys_ofile [users [user_id].u_ofile [cfd]].f_inode;
    iput ( inode );
    sys_ofile [users [user_id].u_ofile [cfd]].f_count--;
    users [user_id].u_ofile [cfd] = SYSOPENFILE + 1;
}

