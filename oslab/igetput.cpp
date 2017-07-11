/*»ñÈ¡ÊÍ·Åi½ÚµãÄÚÈÝ³ÌÐòiget()/iput()*/
#include <stdio.h>
#include "FILESYS.h"
#include<malloc.h>
#include"ballocfre.h"
#include"iallfre.h"
//hash队列用来存放内存inode。
//但是为什么要弄成队列的样子呢？方便查找？inode编号是唯一的, 一个hash表128个队列，假设内存有256个inode
//举个栗子，假设要找第129个inode：
//inode编号->hash映射得到hash队列编号1->1次就能找到
//如果不用hash队列，假设用数组, 按照线性存放，要找129次
//查找内存inode的操作经常用到，hash表的作用就是为了提高查找内存inode的效率
struct inode * iget(unsigned int disk_index_node_id)    /* iget( ) */
{
    int  inode_id, existed = 0;
	long addr;
	struct inode *temp, *newinode;
    inode_id = disk_index_node_id % NumOfHashIndexNode;//计算内存结点应该在第几个哈希队列里 NHINO=128
    if ( hash_index_node [inode_id].i_forw == NULL ) //若该哈希队列为空，内存结点一定未被创建
    {
        existed = 0;
    }
	else//若不为空，从该哈希队列头开始查找
	{
		temp = hash_index_node [inode_id].i_forw;
		while (temp)
		{
			if (temp->i_ino == disk_index_node_id ) // 若找到 mkdir时，对象已经存在，引用计数会加1
			{
				existed = 1;
				temp->i_count++;
				return temp;//返回该内存结点指针
			}
			else    /*not existed */
				temp = temp->i_forw;
		};
	}
	/* 若没有找到 */
	/* 1. 计算该磁盘i结点在文件卷中的位置 */
	addr = DiskIndexNodeStart + disk_index_node_id * DiskIndexNodeSize;
	/* 2. 分配一个内存i结点 */
	newinode = (struct inode *) malloc(sizeof(struct inode));
	/* 3. 用磁盘i结点初始化内存i结点 */
	fseek(fd, addr, SEEK_SET);
	fread(newinode, DiskIndexNodeSize, 1, fd);
	/* 4. 将内存i结点链入相应的哈希队列里*/
	newinode->i_forw = hash_index_node [inode_id].i_forw;
	hash_index_node[inode_id].i_forw = newinode;
	newinode->i_back = newinode;

	if (newinode->i_forw) {
		newinode->i_forw->i_back = newinode;
	}
	/*5. 初始化内存i结点的其他数据项 */
	hash_index_node[inode_id].i_forw = newinode;

	/* 5.initialize the mode */
	newinode->i_count = 1;//引用计数设为1
	newinode->i_flag = 0;   /* 表示未更新 */
	newinode->i_ino = disk_index_node_id;

	return newinode;
}

int iput(struct inode * put_inode) /* iput ( ) */
{
	long addr;
	unsigned int block_num;
	int i;

	if ( put_inode->i_count>1)//若引用计数>1
	{
        put_inode->i_count--;//引用计数减1
		return 0;
	}
	else
	{
		if ( put_inode->di_number != 0)//若联结计数不为0
		{
			/* 把内存i结点的内容写回磁盘i结点 */
			addr = DiskIndexNodeStart + put_inode->i_ino * DiskIndexNodeSize;
			fseek(fd, addr, SEEK_SET);
			fwrite(put_inode, DiskIndexNodeSize, 1, fd);//todo 修改了写入方式
		}
		else
		{
			/* 删除磁盘i结点和文件对应的物理块 */ 
			block_num = put_inode->di_size / BLOCKSIZ;
			for (i = 0;i<(int)block_num;i++)
			{
				bfree( put_inode->di_addr[i]);

			}
			ifree( put_inode->i_ino);
		};

		/* 释放内存i结点 */
		{
			int inodeid;
			inodeid = ( put_inode->i_ino) % NumOfHashIndexNode;//找到所在的哈希队列

											  /* 从该哈希队列里删除 */
			if (hash_index_node[inodeid].i_forw == put_inode )
			{
				hash_index_node[inodeid].i_forw = put_inode->i_forw;
				if ( put_inode->i_forw)
                    put_inode->i_forw->i_back = put_inode->i_forw;
			}
			else
			{
                put_inode->i_back->i_forw = put_inode->i_forw;
				if ( put_inode->i_forw)
                    put_inode->i_forw->i_back = put_inode->i_back;
			}
		}
		free( put_inode );
	}
	return 0;
}