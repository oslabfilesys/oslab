//数据块的分配和回收
#include <stdio.h>
#include "FILESYS.h"

static unsigned int block_buf[BLOCKSIZ];//BLOCKSIZ/sizeof(int)
/**********************************************************
函数：balloc
功能：维护超级块中的空闲数据栈，分配空闲数据块，并返回其块号
***********************************************************/
unsigned int balloc()//分配数据块
{
	unsigned int free_block;
	unsigned int i,flag;
	//如果没有空闲盘块
	if ( file_system.s_nfree == 0)
	{
		printf("\nDisk Full!!! \n");
		return DISKFULL;
	};
    free_block = file_system.s_free [file_system.s_pfree]; //取堆栈中的盘块号（从3号物理块开始分配）
    if ( file_system.s_pfree == NICFREE - 1 ) {           //如果堆栈只剩一个块
        fseek ( fd, DATASTART + ( free_block ) *BLOCKSIZ, SEEK_SET );//从中读取下一组块号
        fread ( block_buf, 1, BLOCKSIZ, fd );
        //从中读取下一组块号
        for ( i = 0; i<NICFREE; i++ )
            file_system.s_free [i] = block_buf [i];
        file_system.s_pfree = 0;             //设置堆栈指针
    }
    else {//如果堆栈中大于一个盘块
        file_system.s_pfree++;               //修改堆栈指针
    }
    file_system.s_nfree--;                  //修改总块数
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