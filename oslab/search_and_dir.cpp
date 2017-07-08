#include <string.h>
#include <stdio.h>
#include "FILESYS.h"
#include "search_and_dir.h"
#include"igetput.h"
#include"ballocfre.h"
#include"access.h"
#include"iallfre.h"
unsigned int namei(char *name) /* namei */

{
	int i, notfound = 1;
	for (i = 0; ((i<directory.size) && (notfound)); i++)
	{
		if ((!strcmp( directory.direct[i].d_name,name)) && ( directory.direct[i].d_ino != 0))
			return directory.direct[i].d_ino;   /* find */
	}
	return 0;   /* notfind */
}

unsigned int iname(char *name)	/* iname 搜索函数*/
{
	int i, notfound = 1;
	for (i = 0; ((i<DIRNUM) && (notfound)); i++)
		if ( directory.direct[i].d_ino == 0)
		{
			notfound = 0;
			break;
		}

	if (notfound)
	{
		printf("\n The current directory is full!\n");
		return 0;
	}
	else
	{
		strcpy_s( directory.direct[i].d_name,name);
        directory.direct[i].d_ino = 1;
		return i;
	}
}
void _dir()	/* _dir 显示列表函数 */
{
	unsigned short di_mode;
	int i, j,k,one;
	struct inode * temp_inode;
	printf("\nCURRENT DIRECTORY:dir.size=%d\n", directory.size);
	for (i = 0; i<directory.size; i++)
	{
		if ( directory.direct[i].d_ino != DIEMPTY)
		{
			printf("%20s", directory.direct[i].d_name);
			temp_inode = iget( directory.direct[i].d_ino);
			di_mode = temp_inode->di_mode;
			if (temp_inode->di_mode&DIFILE)
				printf("f");
			else
				printf("d");
			for (j = 0; j<9; j++)
			{
				one = di_mode % 2;
				di_mode = di_mode / 2;
				if (one) 
					printf("x");
				else 
					printf("-");
			}
			if (temp_inode->di_mode &DIFILE)
			{
				printf("%ld", temp_inode->di_size);
				printf("block chain:");
				for (k = 0; k<temp_inode->di_size / BLOCKSIZ + 1; k++)
					printf("%d", temp_inode->di_addr[k]);
				printf("\n");
			}
			else 
				printf("<dir>block chain:%d\n", directory.direct[i].d_ino);
			iput(temp_inode);
		}
	}
}

void mkdir(char *dirname)	/* mkdir 目录创建函数*/
{
	int dirid, dirpos;
	struct inode * inode;
	struct direct buf[BLOCKSIZ / (DIRSIZ + 2)];
	unsigned int block;
	dirid = namei(dirname);
	if (dirid != 0)
	{
		inode = iget(dirid);
		if (inode->di_mode & DIDIR)
			printf("\n%s directory already existed!\n",dirname);
		else
			printf("\n%s is a file name, &can't create a dir the same name\n", dirname);
		iput(inode);
		return;
	}
	dirpos = iname(dirname);
	inode = ialloc();
	dirid=inode->i_ino;
    directory.direct[dirpos].d_ino = inode->i_ino;
    directory.size++;
	/*	fill the new dir buf */
	strcpy_s(buf[0].d_name,".");
	buf[0].d_ino = dirid;
	strcpy_s(buf[1].d_name,"..");
	buf[1].d_ino = cur_path_inode->i_ino;
	buf[2].d_ino = 0;
	block = balloc();
	fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fwrite(buf, 1, BLOCKSIZ, fd);
	inode->di_size = 2 * (DIRSIZ + 2);
	inode->di_number = 1;
	inode->di_mode = user[user_id].u_default_mode|DIDIR;
	inode->di_uid = user[user_id].u_uid;
	inode->di_gid = user[user_id].u_gid;
	inode->di_addr[0] = block;
	iput(inode);
	return;
}
void chdir(char *dirname) /* chdir 改变当前目录用函数 */
{
	unsigned int dirid;
	struct inode * inode;
	unsigned short block;
	int i, j, low = 0, high = 0;
	dirid = namei(dirname);
	if (dirid == 0)
	{
		printf("\n%s does not exist!\n", dirname);
		return;
	}
	inode = iget(dirid);
	if (!access(user_id, inode, user[user_id].u_default_mode))
	{
		printf("\nhas not access to the directory %s\n", dirname);
		iput(inode);
		return;
	}
	/* pack the current directory */
	for (i = 0; i<directory.size; i++)
	{
		for (j = 0; j<DIRNUM; j++)
			if ( directory.direct[j].d_ino == 0)
				break;
		memcpy(&directory.direct[j], &directory.direct[i], DIRSIZ + 2);
        directory.direct[j].d_ino = 0;
	}
	/*	write back the current directory */
	for (i = 0; i<cur_path_inode->di_size / BLOCKSIZ + 1; i++)
	{
		bfree(cur_path_inode->di_addr[i]);
	}
	for (i = 0; i<directory.size; i += BLOCKSIZ / (DIRSIZ + 2))
	{
		block = balloc();
		cur_path_inode->di_addr[i] = block;
		fseek(fd, DATASTART + block*BLOCKSIZ, SEEK_SET);
		fwrite(&directory.direct[0], 1, BLOCKSIZ, fd);
	}
	cur_path_inode->di_size = directory.size*(DIRSIZ + 2);
	iput(cur_path_inode);
	cur_path_inode = inode;
    directory.size = inode->di_size / (DIRSIZ + 2);
	/*	read the change dir from disk */
	j = 0;
	for (i = 0; i<inode->di_size / BLOCKSIZ + 1; i++)
	{
		fseek(fd, DATASTART + inode->di_addr[i] * BLOCKSIZ, SEEK_SET);
		fread(&directory.direct[0], 1, BLOCKSIZ, fd);
		j += BLOCKSIZ / (DIRSIZ + 2);
	}
return;
}