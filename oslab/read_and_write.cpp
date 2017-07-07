#include <stdio.h>
#include "FILESYS.h"
#include "read_and_write.h"
unsigned int read_file(int fd1, char *buf, unsigned int size)
{
	unsigned long off;
	int block, block_off, i, j;
	struct inode * inode;
	char * temp_buf;
	inode = sys_ofile[user[user_id].u_ofile[fd1]].f_inode;
	if (!(sys_ofile[user[user_id].u_ofile[fd1]].f_flag & FREAD))
	{
		printf("\nthe file is not opened for read\n");
		return 0;
	}
	temp_buf = buf;
	off = sys_ofile[user[user_id].u_ofile[fd1]].f_off;
	if ((off + size)>inode->di_size) 
		size = inode->di_size - off;
	block_off = off % BLOCKSIZ;
	block = off / BLOCKSIZ;
	if (block_off + size<BLOCKSIZ)
	{
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
		fread(buf, 1, size, fd);
		return size;
	}
	fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
	fread(temp_buf, 1, BLOCKSIZ - block_off, fd);
	temp_buf += BLOCKSIZ - block_off;
	j = (inode->di_size - off - block_off) / BLOCKSIZ;
	for (i = 0; i<(size - block_off) / BLOCKSIZ; i++)
	{
		fseek(fd, DATASTART + inode->di_addr[j + i] * BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, BLOCKSIZ, fd);
		temp_buf += BLOCKSIZ;
	}

	block_off = (size - block_off) % BLOCKSIZ;
	block = inode->di_addr[off + size / BLOCKSIZ + 1];
	fseek(fd, DATASTART + block*BLOCKSIZ, SEEK_SET);
	fread(temp_buf, 1, block_off, fd);
	sys_ofile[user[user_id].u_ofile[fd1]].f_off += size;
	return size;
}

unsigned int write_file(int fd1,char *buf,unsigned int size)
{
	unsigned long off;
	int block, block_off, i, j, k = 0;
	struct inode * inode;
	char * temp_buf;
	inode = sys_ofile[user[user_id].u_ofile[fd1]].f_inode;
	temp_buf = buf;

	off = sys_ofile[user[user_id].u_ofile[fd1]].f_off;
	block_off = off % BLOCKSIZ;
	block = off / BLOCKSIZ;

	if (block_off + size < BLOCKSIZ)
	{
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
		fwrite(buf, 1, size, fd);
		printf("Write success!\n");
		return size;
	}
	if (sys_ofile[user[user_id].u_ofile[fd1]].f_flag == FAPPEND)
	{
	fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
	fwrite(temp_buf, 1, BLOCKSIZ - block_off, fd);
	temp_buf += BLOCKSIZ - block_off;
	k = 1;
    }
	for (i = 0; i<(size -k*(BLOCKSIZ- block_off)) / BLOCKSIZ; i++)
	{
		inode->di_addr[block + 1 + i] = balloc();
		fseek(fd, DATASTART + inode->di_addr[block + k + i] * BLOCKSIZ, SEEK_SET);
		fwrite(temp_buf, 1, BLOCKSIZ, fd);
		temp_buf += BLOCKSIZ;
	}
	block_off = (size - k*(BLOCKSIZ - block_off)) % BLOCKSIZ;
	block = inode->di_addr[block+k+i]=balloc();
	fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fwrite(temp_buf, 1, block_off, fd);
	sys_ofile[user[user_id].u_ofile[fd1]].f_off += size;
	inode->di_size = sys_ofile[user[user_id].u_ofile[fd1]].f_off;
	return size;
}