#include <stdio.h>
#include "FILESYS.h"

static unsigned int block_buf[BLOCKSIZ];
unsigned int balloc()
{
	unsigned int free_block, free_block_num;
	unsigned int i,flag;
	if (filsys.s_nfree == 0)
	{
		printf("\nDisk Full!!! \n");
		return DISKFULL;
	};
	free_block = filsys.s_free[filsys.s_pfree];
	if(filsys.s_pfree==NICFREE-1)
	{
		fseek(fd, DATASTART + BLOCKSIZ*(562-filsys.s_nfree), SEEK_SET);////filsys.s_free[NICFREE-1]+1指向下一个block组的地址块 fread(block_buf,1,BLOCKSIZ,fd);
		fread(block_buf, 1, BLOCKSIZ, fd);
		free_block_num = block_buf[NICFREE];
		for(i = 0; i<free_block; i++)
		{
			filsys.s_free[NICFREE-i-1] = block_buf[i];
		}//将待用block组的地址读入超级块
		filsys.s_pfree = NICFREE - free_block_num;
	}
	else
	{
		filsys.s_pfree++;
	}
	filsys.s_nfree--;
	filsys.s_fmod = SUPDATE;
	return free_block;
}
int bfree(unsigned int block_num)
{
	int i;
	if (filsys.s_pfree == NICFREE - 1)//表示回收的block已经可以组成一个block组了
	{
		for (i = 0; i<NICFREE; i++)
		{
			block_buf[i] = filsys.s_free[NICFREE - 1 - i];
		}
		filsys.s_pfree = 0;
		fseek(fd, DATASTART + BLOCKSIZ*(filsys.s_free[0]), SEEK_SET); //filsys.s_free[0]为当前BLOCK组的地址块
		fwrite(block_buf, 1, BLOCKSIZ, fd);
	}
	else filsys.s_pfree++;
	filsys.s_nfree++;
	filsys.s_fmod = SUPDATE;
	return 0;
}