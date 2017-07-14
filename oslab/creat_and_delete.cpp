#include "creat_and_delete.h"
#include"igetput.h"

#include"creat_and_delete.h"
#include"search_and_dir.h"
#include"ballocfre.h"
#include"iallfre.h"
#include"access.h"

//创建完成就打开了
int create_file(unsigned int user_id, const char *filename,unsigned short mode)//用户ID；文件名；存取权限
{
	unsigned int di_ith, di_ino;
	struct inode * inode;
	int i, j;
	di_ino = search_directory_or_file_id_by_name (filename);//是否已经存在文件或者文件夹
	if (di_ino != 0)	/* already existed */
	{
		inode = iget(di_ino);
		if (access(user_id, inode, mode) == 0)//访问控制权限
		{
			iput(inode);
			printf("\ncreate access not allowed");
			return 0;
		}
		else//有访问权限而且文件存在
		{
			printf("\ndo you want to replace the old file?(y/n)\n");
			if (getchar() == 'y')
			{
				/* free all the block of the old file */
				for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++)
				{
					bfree(inode->di_addr[i]);
				}

				for (i = 0; i < SYSOPENFILE; i++)
					if (sys_ofile[i].f_inode == inode)
					{
						sys_ofile[i].f_off = 0;
					}
				for (i = 0; i < NOFILE; i++)
					if (users[user_id].u_ofile[i] == SYSOPENFILE + 1)
					{
						users[user_id].u_uid = inode->di_uid;
					
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
		di_ith = put_the_name_to_current_dir (filename);//写入到了drectory的第几项
        directory.size++;
        directory.direct[di_ith].d_ino = inode->i_ino;

        inode->di_mode = FILE_TYPE;//默认自己权限
        inode->other_mode = READ_AB;// 他人默认可读
        inode->di_uid = users [user_id].u_uid;
		inode->di_size = 0;//初始文件大小
		inode->di_number = 1;//关联文件数
        int new_block = balloc ( );
        if ( new_block != DISKFULL )
        {
            inode->di_addr [0] = new_block;
        }
		for (i = 0; i < SYSOPENFILE; i++)//系统打开文件数
			if (sys_ofile[i].f_count == 0)
			{
				break;
			}
		for (j = 0; j < NOFILE; j++)//用户打开的文件数
			if ( users [user_id].u_ofile[j] == SYSOPENFILE + 1)
			{
				break;
			}
        if ( i == SYSOPENFILE || j == NOFILE )
        {
            printf ( "用户打开文件超过限制！\n" );
            return UserOpenFileOverflow;
        }
        users [user_id].u_ofile[j] = i;
		sys_ofile[i].f_flag = mode;
		sys_ofile[i].f_count ++;
		sys_ofile[i].f_off = 0;
		sys_ofile[i].f_inode = inode;
        cout << "create ok!" << endl;
		return j;
	}
}

void delete_file ( const char * filename )//删除文件
{
    unsigned int dinodeid, i;
    struct inode *inode;
    dinodeid = search_directory_or_file_id_by_name ( filename );
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

