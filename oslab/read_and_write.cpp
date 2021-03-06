#include <stdio.h>
#include "FILESYS.h"
#include "read_and_write.h"
#include"ballocfre.h"
unsigned int read_file(int fd1,char *buf,unsigned int size)//fd1只是一个形参，相当于第几个文件

{
	unsigned long off;
	int block, block_off, i, j;
	struct inode * inode;
	char * temp_buf;
	inode = sys_ofile[users [user_id].u_ofile[fd1]].f_inode;
	if (!(sys_ofile[users [user_id].u_ofile[fd1]].f_flag & READ_AB))
	{
		printf("\nthe file is not opened for read\n");
		return 0;
	}
	temp_buf = buf;
	off = sys_ofile[users [user_id].u_ofile[fd1]].f_off;
    if ( ( off + size ) > inode->di_size ) {
        cout << "file do not hve enough bytes to read" << endl;
        return OVERFLOW;
    }
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
	sys_ofile[users [user_id].u_ofile[fd1]].f_off += size;
	return size;
}

unsigned int write_file(int fd1, char *buf,unsigned int size)
{
	unsigned long off;
	int block, block_off, i, j, k = 0;
	struct inode * inode;
	char * temp_buf;
	inode = sys_ofile[users [user_id].u_ofile[fd1]].f_inode;
	temp_buf = buf;

	off = sys_ofile[users [user_id].u_ofile[fd1]].f_off;
	block_off = off % BLOCKSIZ;
	block = off / BLOCKSIZ;
    if ( sys_ofile [users [user_id].u_ofile [fd1]].f_flag & WriteAble ) {


        if ( block_off + size < BLOCKSIZ )
        {
            fseek ( fd, DATASTART + inode->di_addr [block] * BLOCKSIZ + block_off, SEEK_SET );
            fwrite ( buf, 1, size, fd );
            printf ( "Write success!\n" );

            sys_ofile [users [user_id].u_ofile [fd1]].f_off += size;
            inode->di_size = sys_ofile [users [user_id].u_ofile [fd1]].f_off;
        }

        else
        {
            fseek ( fd, DATASTART + inode->di_addr [block] * BLOCKSIZ + block_off, SEEK_SET );
            fwrite ( temp_buf, 1, BLOCKSIZ - block_off, fd );
            temp_buf += BLOCKSIZ - block_off;
            k = 1;
            //写完没满的第一块，下面是剩下的

            for ( i = 0; i < ( size - ( BLOCKSIZ - block_off ) ) / BLOCKSIZ; i++ )
            {
                inode->di_addr [block + 1 + i] = balloc ( );
                fseek ( fd, DATASTART + inode->di_addr [block + 1 + i] * BLOCKSIZ, SEEK_SET );
                fwrite ( temp_buf, 1, BLOCKSIZ, fd );
                temp_buf += BLOCKSIZ;
            }//直到最后一块

            block_off = ( size - ( BLOCKSIZ - block_off ) ) % BLOCKSIZ;//最后剩下的一丢丢
            block = inode->di_addr [block + 1 + i] = balloc ( );
            fseek ( fd, DATASTART + block * BLOCKSIZ, SEEK_SET );
            fwrite ( temp_buf, 1, block_off, fd );
            sys_ofile [users [user_id].u_ofile [fd1]].f_off += size;
            inode->di_size = sys_ofile [users [user_id].u_ofile [fd1]].f_off;
            printf ( "Write success!\n" );
            return size;
        }
    }
    else
    {
        printf ( "access denied,please check your permisssion" );
        return 0;
    }
}