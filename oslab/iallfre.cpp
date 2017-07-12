#include <stdio.h>
#include "FILESYS.h"
#include"igetput.h"

static unsigned char block_buf[51];
struct inode * ialloc()
{
	struct inode * temp_inode;
	unsigned int cur_di;
	int i, count, block_end_flag;

	if ( file_system.surplus_point_to_inode == NICINOD)//inode数组分完了，该重新准备了
	{
		count = 0;//总数初始化
		block_end_flag = 1;
        file_system.surplus_point_to_inode = 0;
		cur_di = file_system.s_rinode;
        fseek ( fd, DiskIndexNodeStart + DiskIndexNodeSize*cur_di, SEEK_SET );
        fread ( block_buf, 1, 51, fd );
        for ( int i = 0; i < 50; i++ )
        {
            file_system.s_inode [i] = block_buf [i];
        }
        file_system.s_rinode = block_buf [51];

	}
    if ( file_system.surplus_nunmber_of_inode == 0 ) {
        temp_inode = nullptr;
        printf ( "inode none!\n" );
    }
    else
    {
        temp_inode = iget ( file_system.s_inode [file_system.surplus_point_to_inode] );
        file_system.surplus_point_to_inode = file_system.surplus_point_to_inode + 1;
        file_system.surplus_nunmber_of_inode--;
        file_system.s_fmod = SUPDATE;
        for ( int i = 0; i < NADDR; i++ )
        {
            temp_inode->di_addr [i] = NotHaveInode;
        }
    }
	return temp_inode;
}


void ifree(unsigned dinodeid)	 /* ifree */
{
    file_system.surplus_nunmber_of_inode++;
	if ( file_system.surplus_point_to_inode != 0)    /* notfull */
	{
        file_system.s_inode[file_system.surplus_point_to_inode--] = dinodeid;
	}
	else /* full */
	{
        unsigned char buff [51];
        for ( int i = 0; i < 50; i++ )
        {
            buff [i] = file_system.s_inode [i];
        }
        buff [50] = file_system.s_rinode;
        file_system.s_rinode = file_system.s_inode [0];//将新的一组保存到buff[0】
        fseek ( fd, DiskIndexNodeStart + DiskIndexNodeSize*file_system.s_rinode, SEEK_SET );
        fwrite ( buff, 1, 51, fd );//保存完成

        file_system.surplus_point_to_inode = 49;
        file_system.s_inode [file_system.surplus_point_to_inode] = dinodeid;//新的一组
	}
}