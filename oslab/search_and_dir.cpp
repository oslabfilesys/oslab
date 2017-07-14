#include <string.h>
#include <stdio.h>
#include "FILESYS.h"
#include "search_and_dir.h"
#include"igetput.h"
#include"ballocfre.h"
#include"access.h"
#include"iallfre.h"
unsigned int search_directory_or_file_id_by_name( const char *name) /* namei */

{
	int i, notfound = 1;
	for (i = 0; ((i<directory.size) && (notfound)); i++)
	{
		if ((!strcmp( directory.direct[i].d_name,name)) && ( directory.direct[i].d_ino != 0))
			return directory.direct[i].d_ino;   /* find */
	}
	return 0;   /* notfind */
}

unsigned int put_the_name_to_current_dir( const char *name)	/* iname 搜索函数*/
{
	int i, notfound = 1;
	for (i = 0; i < DIRNUM; i++)
		if ( directory.direct[i].d_ino == 0)
		{
			notfound = 0;
			break;
		}

	if (notfound)
	{
		printf("\n The current directory is full!\n");
		return 0;
	}
	else
	{
		strcpy_s( directory.direct[i].d_name,name);
        directory.direct[i].d_ino = 1;//？
		return i;
	}
}

void _dir()	/* _dir 显示列表函数 */
{
	unsigned short di_mode;
	int i, j,k,one;
	struct inode * temp_inode;
	printf("\nCURRENT DIRECTORY:dir.size=%d\n", directory.size);
	for (i = 0; i<directory.size; i++)
	{
		if ( directory.direct[i].d_ino != DIEMPTY)
		{
			printf("%20s", directory.direct[i].d_name);
			temp_inode = iget( directory.direct[i].d_ino);
			di_mode = temp_inode->di_mode;
			if (temp_inode->di_mode&FILE_TYPE)
				printf("f");
			else
				printf("d");
			for (j = 0; j<9; j++)
			{
				one = di_mode % 2;
				di_mode = di_mode / 2;
				if (one) 
					printf("x");
				else 
					printf("-");
			}
			if (temp_inode->di_mode &FILE_TYPE )
			{
				printf("%ld", temp_inode->di_size);
				printf("block chain:");
				for (k = 0; k<temp_inode->di_size / BLOCKSIZ + 1; k++)
					printf("%d ", temp_inode->di_addr[k]);
				printf("\n");
			}
			else 
				printf("<dir>block chain:%d\n", temp_inode->di_addr [0] );
			iput(temp_inode);
		}
	}
}

void mkdir( const char *dirname)	/* mkdir 目录创建函数*/
{
	int dirid, dirpos;
	struct inode * inode;
	struct direct direct_buf[MaxNumberOfDirOrFileInADirectory];
	unsigned int block;
	dirid = search_directory_or_file_id_by_name (dirname);
	if (dirid != 0)
	{
		inode = iget(dirid);
		if (inode->di_mode & DIR_TYPE)
			printf("\n%s directory already existed!\n",dirname);
		else
			printf("\n%s is a file name, &can't create a dir the same name\n", dirname);
		iput(inode);
		return;
	}
	dirpos = put_the_name_to_current_dir (dirname);//判断满没满（128以后会有问题）
    if ( dirpos == 0 )
    {
        return;
    }

	inode = ialloc();
	dirid=inode->i_ino;
    directory.direct[dirpos].d_ino = dirid;
    directory.size++;
    cur_path_inode->di_size = directory.size * 20;

	/*	fill the new dir buf */
	strcpy_s( direct_buf [0].d_name,".");//增加当前目录的目录项
    direct_buf [0].d_ino = dirid;
	strcpy_s( direct_buf [1].d_name,"..");//增加上一级的目录项
    direct_buf [1].d_ino = cur_path_inode->i_ino;
    for ( int i = 2; i <MaxNumberOfDirOrFileInADirectory; i++ )
    {
        strcpy_s ( direct_buf [i].d_name, "   " );
        direct_buf [i].d_ino = 0;
    }

	block = balloc();
	fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
    fwrite ( direct_buf, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct) , fd );
	inode->di_size = 2 * (20);
	inode->di_number = 1;
	inode->di_mode = DIR_TYPE;//默认自己权限
    inode->other_mode = READ_AB;// 他人默认可读
	inode->di_uid = users [user_id].u_uid;
	inode->di_addr[0] = block;
	iput(inode);
    printf ( "create dir %s success!\n", dirname );
	return;
}

void chdir( const char *dirname) /* chdir 改变当前目录用函数 */
{
	unsigned int dirid;
	struct inode * inode;
	unsigned short block;
    struct direct direct_buf [32];
	int i, j, low = 0, high = 0;
	dirid = search_directory_or_file_id_by_name (dirname);
	if (dirid == 0)
	{
		printf("\n%s does not exist!\n", dirname);
		return;
	}
	inode = iget(dirid);
	if (!access(user_id, inode, users [user_id].u_default_mode))
	{
		printf("\nhas not access to the directory %s\n", dirname);
		iput(inode);
		return;
	}

    /*	write back the current directory */
    fseek ( fd, DATASTART + BLOCKSIZ*cur_path_inode->di_addr [0], SEEK_SET );
    fwrite ( directory.direct, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct ), fd );
    iput ( cur_path_inode );

	/* pack the current directory */
    fseek ( fd, DATASTART + BLOCKSIZ*inode->di_addr [0], SEEK_SET );
    fread ( directory.direct, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct ), fd );
    for ( int i = 0; i < MaxNumberOfDirOrFileInADirectory; i++ )
    {
        if ( directory.direct [i].d_ino == 0 ) {
            directory.size = i;
            break;
        }
    }

	cur_path_inode = inode;
    printf ( "change to dir %s success!\n", dirname );
return;
}