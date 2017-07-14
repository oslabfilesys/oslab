#include "creat_and_delete.h"
#include"igetput.h"

#include"creat_and_delete.h"
#include"search_and_dir.h"
#include"ballocfre.h"
#include"iallfre.h"
#include"access.h"

//������ɾʹ���
int create_file(unsigned int user_id, const char *filename,unsigned short mode)//�û�ID���ļ�������ȡȨ��
{
	unsigned int di_ith, di_ino;
	struct inode * inode;
	int i, j;
	di_ino = search_directory_or_file_id_by_name (filename);//�Ƿ��Ѿ������ļ������ļ���
	if (di_ino != 0)	/* already existed */
	{
		inode = iget(di_ino);
		if (access(user_id, inode, mode) == 0)//���ʿ���Ȩ��
		{
			iput(inode);
			printf("\ncreate access not allowed");
			return 0;
		}
		else//�з���Ȩ�޶����ļ�����
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
		di_ith = put_the_name_to_current_dir (filename);//д�뵽��drectory�ĵڼ���
        directory.size++;
        directory.direct[di_ith].d_ino = inode->i_ino;

        inode->di_mode = FILE_TYPE;//Ĭ���Լ�Ȩ��
        inode->other_mode = READ_AB;// ����Ĭ�Ͽɶ�
        inode->di_uid = users [user_id].u_uid;
		inode->di_size = 0;//��ʼ�ļ���С
		inode->di_number = 1;//�����ļ���
        int new_block = balloc ( );
        if ( new_block != DISKFULL )
        {
            inode->di_addr [0] = new_block;
        }
		for (i = 0; i < SYSOPENFILE; i++)//ϵͳ���ļ���
			if (sys_ofile[i].f_count == 0)
			{
				break;
			}
		for (j = 0; j < NOFILE; j++)//�û��򿪵��ļ���
			if ( users [user_id].u_ofile[j] == SYSOPENFILE + 1)
			{
				break;
			}
        if ( i == SYSOPENFILE || j == NOFILE )
        {
            printf ( "�û����ļ��������ƣ�\n" );
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

void delete_file ( const char * filename )//ɾ���ļ�
{
    unsigned int dinodeid, i;
    struct inode *inode;
    dinodeid = search_directory_or_file_id_by_name ( filename );
    if ( dinodeid != 0 ) //�ļ��Ƿ����
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

