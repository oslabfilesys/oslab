/*获取释放i节点内容程序iget()/iput()*/
#include <stdio.h>
#include "FILESYS.h"
#include<malloc.h>
#include"ballocfre.h"
#include"iallfre.h"

struct inode * iget(unsigned int dinodeid)    /* iget( ) */
{
	int existed = 0, inodeid;
	long addr;
	struct inode *temp, *newinode;
	inodeid = dinodeid % NHINO;
	if (hinode[inodeid].i_forw == NULL) existed=0;
	else
	{
		temp = hinode[inodeid].i_forw;
		while (temp)
		{
			if (temp->i_ino == inodeid)
				/*existed */
			{
				existed = 1;
				temp->i_count++;
				return temp;
			}
			else    /*not existed */
				temp = temp->i_forw;
		};
	}
	/*	1. calcu late the addr of the dinode in the file sys column */
	addr = DINODESTART + dinodeid * DINODESIZ;
	/*	2. malloc the new mode */
	newinode = (struct inode *) malloc(sizeof(struct inode));
	/*	3.read the dinode to the mode */
	fseek(fd, addr, SEEK_SET);
	fread(&(newinode->di_number), DINODESIZ, 1, fd);
	/* 4.put it into hinode[inodeid] queue */
	newinode->i_forw = hinode[inodeid].i_forw;
	newinode->i_back = newinode;
	if (newinode->i_forw != NULL) {
		newinode->i_forw->i_back = newinode;
	}
	hinode[inodeid].i_forw = newinode;
	/* 5.initialize the mode */
	newinode->i_count = 1;
	newinode->i_flag = 0;    /* flag for not update */
	newinode->i_ino = dinodeid;
	newinode->di_size = 3 * (DIRSIZ + 2);
	if (dinodeid == 3)newinode->di_size = BLOCKSIZ;
	return newinode;
}

int iput(struct inode * pinode) /* iput ( ) */
{
	long addr;
	unsigned int block_num;
	int i;

	if (pinode->i_count>1)
	{
		pinode->i_count--;
		return 0;
	}
	else
	{
		if (pinode->di_number != 0)
		{
			/*write back the mode */
			addr = DINODESTART + pinode->i_ino * DINODESIZ;
			fseek(fd, addr, SEEK_SET);
			fwrite(&pinode->di_number, DINODESIZ, 1, fd);
		}
		else
		{
			/*	rm the mode & the block of the file in the disk */
			block_num = pinode->di_size / BLOCKSIZ;
			for (i = 0;i<(int)block_num;i++)
			{
				bfree(pinode->di_addr[i]);

			}
			ifree(pinode->i_ino);
		};

		/*	free the mode in the memory */
		if (pinode->i_forw == NULL)
			pinode->i_back->i_forw = NULL;
		else {
			pinode->i_forw->i_back = pinode->i_back;
			pinode->i_back->i_forw = pinode->i_forw;
		};
		ifree(pinode->i_ino);
	};
}