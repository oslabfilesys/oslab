//数据块的分配和回收
#include <stdio.h>
#include "FILESYS.h"

static unsigned int block_buf[BLOCKSIZ];//BLOCKSIZ/sizeof(int)
unsigned int balloc()//分配数据块
{
	unsigned int free_block, free_block_num;
	unsigned int i,flag;
	if ( file_system.s_nfree == 0)
	{
		printf("\nDisk Full!!! \n");
		return DISKFULL;
	};
	i = file_system.s_pfree;
	flag = (i == 0);
	if (flag)//该block组全部用了
	{
		fseek(fd, DATASTART + BLOCKSIZ*(file_system.s_free[NICFREE - 1] + 1), SEEK_SET);//file_system.s_free[NICFREE - 1]+1指向下一个block组的地址块
		fread(block_buf, 1, BLOCKSIZ, fd);
		for (i = 0;i < NICFREE;i++) {
			file_system.s_free[i] = block_buf[i];
		}//将待用的block组的地址块读入超级块
		file_system.s_pfree = NICFREE - 1;
		free_block = file_system.s_free[file_system.s_pfree];
	}

	//if( file_system.s_pfree==NICFREE-1)
	//{
	//	fseek(fd, DATASTART + BLOCKSIZ*(562- file_system.s_nfree), SEEK_SET);////filsys.s_free[NICFREE-1]+1指向下一个block组的地址块 fread(block_buf,1,BLOCKSIZ,fd);
	//	fread(block_buf, 1, BLOCKSIZ, fd);
	//	free_block_num = block_buf[NICFREE];
	//	for(i = 0; i<free_block; i++)
	//	{
 //           file_system.s_free[NICFREE-i-1] = block_buf[i];
	//	}//将待用block组的地址读入超级块
 //       file_system.s_pfree = NICFREE - free_block_num;
	//}
	else
	{
		free_block = file_system.s_free[file_system.s_pfree];
		file_system.s_pfree--;
	}
    file_system.s_nfree--;
    file_system.s_fmod = SUPDATE;
	return free_block;
}
int bfree(unsigned int block_num)
{
	int i;
	if ( file_system.s_pfree == NICFREE - 1)//表示回收的block已经可以组成一个block组了
	{
		for (i = 0; i<NICFREE; i++)
		{
			block_buf[i] = file_system.s_free[NICFREE - 1 - i];
		}
        file_system.s_pfree = 0;
		fseek(fd, DATASTART + BLOCKSIZ*( file_system.s_free[0]), SEEK_SET); //filsys.s_free[0]为当前BLOCK组的地址块
		fwrite(block_buf, 1, BLOCKSIZ, fd);
	}
	else file_system.s_pfree++;
    file_system.s_nfree++;
    file_system.s_fmod = SUPDATE;
	return 0;
}