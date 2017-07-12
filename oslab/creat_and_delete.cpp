#include "creat_and_delete.h"
#include"igetput.h"

#include"creat_and_delete.h"
#include"search_and_dir.h"
#include"ballocfre.h"
#include"iallfre.h"
#include"access.h"
int create_file(unsigned int user_id, char *filename,unsigned short mode)//用户ID；文件名；存取权限
{
	unsigned int di_ith, di_ino;
	struct inode * inode;
	int i, j;
	di_ino = namei(filename);  // 磁盘索引节点赋值是否为空
	if (di_ino != 0)	/* already existed */
	{
		inode = iget(di_ino);
		if (access(user_id, inode, mode) == 0)//访问控制权限
		{
			iput(inode);
			printf("\ncreate access not allowed");
			return 0;
		}
		else
		{
			printf("\ndo you want to replace the old file?(y/n)\n");
			if (getchar() == 'y')
			{
				/* free all the block of the old file */
				for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++)
				{
					bfree(inode->di_addr[i]);
				}
				/* to do: add code here to update the pointer of the sys_file */
				for (i = 0; i < SYSOPENFILE; i++)
					if (sys_ofile[i].f_inode == inode)
					{
						sys_ofile[i].f_off = 0;
					}
				for (i = 0; i < NOFILE; i++)
					if (users[user_id].u_ofile[i] == SYSOPENFILE + 1)
					{
						users[user_id].u_uid = inode->di_uid;
						users[user_id].u_gid = inode->di_gid;
						for (j = 0; j < SYSOPENFILE; j++)
							if (sys_ofile[j].f_count == 0)
							{
								users[user_id].u_ofile[i] = j;
								sys_ofile[j].f_flag = mode;
							}
						return i;
					}
			}
			else
				return 0;
		}
	}
	else /* not existed before */
	{
		inode = ialloc();
		di_ith = iname(filename);
        directory.size++;
        directory.direct[di_ith].d_ino = inode->i_ino;
        directory.direct[di_ith + 1].d_ino = 0;
		inode->di_mode = users [user_id].u_default_mode | DIFILE;
		inode->di_uid = users [user_id].u_uid;
		inode->di_gid = users [user_id].u_gid;
		inode->di_size = file_block;
		inode->di_number = 1;
		for (i = 0; i < SYSOPENFILE; i++)
			if (sys_ofile[i].f_count == 0)
			{
				break;
			}
		for (j = 0; j < NOFILE; j++)
			if ( users [user_id].u_ofile[j] == SYSOPENFILE + 1)
			{
				break;
			}
        users [user_id].u_ofile[j] = i;
		sys_ofile[i].f_flag = mode;
		sys_ofile[i].f_count = 0;
		sys_ofile[i].f_off = 0;
		sys_ofile[i].f_inode = inode;
		return j;
	}
}

void delete_file ( char * filename )//删除文件
{
    unsigned int dinodeid, i;
    struct inode *inode;
    dinodeid = namei ( filename );
    if ( dinodeid != 0 ) //文件是否存在
        inode = iget ( dinodeid );
    else
    {
        printf ( "file not exist!\n" );
        return;
    }
    inode->di_number--;
    for ( i = 0; i < directory.size; i++ )
    {
        if ( directory.direct [i].d_ino == dinodeid )
            break;
    }
    i++;
    while ( directory.direct [i].d_ino != 0 )
    {
        strcpy_s ( directory.direct [i - 1].d_name, directory.direct [i].d_name );
        directory.direct [i - 1].d_ino = directory.direct [i].d_ino;
        i++;
    }
    directory.direct [i - 1].d_ino = 0;
    directory.size = i - 1;
    iput ( inode );
    printf ( "\ndir.size=%d\n", directory.size );

}


