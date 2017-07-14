#include "creat_and_delete.h"
#include"igetput.h"

#include"creat_and_delete.h"
#include"search_and_dir.h"
#include"ballocfre.h"
#include"iallfre.h"
#include"access.h"
#include"open_and_close.h"
#include"read_and_write.h"

//创建完成就打开了
int create_file(struct dir& directory, unsigned int user_id, const char *filename,unsigned short mode)//用户ID；文件名；存取权限
{
	unsigned int di_ith, di_ino;
	struct inode * inode;
	int i, j;
	di_ino = search_directory_or_file_id_by_name ( directory, filename);//是否已经存在文件或者文件夹
	if (di_ino != NOTFOUND)	/* already existed */
	{
        cout << "file have already exist, please delete it!" << endl;
	}
	else /* not existed before */
	{
		inode = ialloc();
		di_ith = put_the_name_to_current_dir (directory ,filename);//写入到了drectory的第几项
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
        iput ( inode );
        cout << "create ok!" << endl;
		return di_ith;
	}
}

void delete_file (struct dir &directory,  const char * filename )//删除文件
{
    unsigned int dinodeid, i;
    struct inode *inode;
    dinodeid = search_directory_or_file_id_by_name ( directory,  filename );
    if ( dinodeid != NOTFOUND ) //文件是否存在
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

int find_file_or_dirctory_pos_in_directory ( struct dir directory, const char* name ) {
    int notfound = 1;
    int i;

    //找到当前文件，一定可以找到
    for ( i = 0; i<directory.size; i++ )
    {
        if ( ( !strcmp ( directory.direct [i].d_name, name ) ) && ( directory.direct [i].d_ino != 0 ) )
            return i;
    }
    return NOTFOUND;
}

void copy_file ( const char * sourch_filename, string dest )

{
    struct inode *sourch_inode, *dest_inode, *temp_inode = nullptr;
    int sourch_inode_id, dest_dir_id, temp_inode_id;
    struct dir directory_temp;
    deque<string> dirs;
    split ( dest, "/", dirs );
    sourch_inode_id = search_directory_or_file_id_by_name ( directory, sourch_filename );
    if ( sourch_inode_id == NOTFOUND ) {
        cout << "file not exist!" << endl;
        return;
    }

    string dest_filename_string = dirs.back ( );
    char *dest_filename;
    int len = dest_filename_string.length ( );
    dest_filename = ( char * ) malloc ( ( len + 1 ) * sizeof ( char ) );
    dest_filename_string.copy ( dest_filename, len, 0 );
    *( dest_filename + len ) = '\0';
    if ( dirs.size ( ) == 1 )
    {//复制到当前目录

        int i = find_file_or_dirctory_pos_in_directory ( directory, sourch_filename );
        int res = search_directory_or_file_id_by_name ( directory, dest_filename );
        if ( res == NOTFOUND )
        {
            create_file ( directory, user_id, dest_filename, DEFAULTMODE );
            int handle;
            handle = open_file ( user_id, sourch_filename, READ_AB );
            if ( handle != OPEN_FAILED ) {
                char *buffer;
                int length = sys_ofile [users [user_id].u_ofile [handle]].f_inode->di_size;
                buffer = ( char* ) malloc ( length + 1 );
                read_file ( handle, buffer, length );
                close_file ( user_id, handle );
                *( buffer + length ) = '\0';
                handle = open_file ( user_id, dest_filename, WRITE_AB );
                if ( handle != OPEN_FAILED ) {
                    write_file ( handle, buffer, length );
                    close_file ( user_id, handle );
                    cout << "copy ok!" << endl;
                }
                else {
                    cout << "something wrong, sorry!" << endl;
                }
                free ( buffer );
            }
            else {
                cout << "something wrong, sorry!" << endl;
            }
        }
        else {
            cout << "dest have exist, please retry" << endl;
        }
    }
    else
    {//复制到其他文件夹
        directory_temp = directory;
        int i;
        for ( i = 0; i < dirs.size ( ) - 1; i++ )
        {
            string temp_dir = dirs [i];
            dest_dir_id = temp_inode_id = search_directory_or_file_id_by_name ( directory_temp, temp_dir.c_str ( ) );
            if ( temp_inode_id == NOTFOUND ) {
                cout << "dest dirctory not exist" << endl;
                return;
            }
            temp_inode = iget ( temp_inode_id );
            if ( !access ( user_id, temp_inode, users [user_id].u_default_mode ) )
            {
                printf ( "\nhas not access to the directory %s\n", temp_dir.c_str ( ) );
                iput ( temp_inode );
                return;
            }
            /* pack the current directory */
            fseek ( fd, DATASTART + BLOCKSIZ*temp_inode->di_addr [0], SEEK_SET );
            fread ( directory_temp.direct, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct ), fd );
            for ( int i = 0; i < MaxNumberOfDirOrFileInADirectory; i++ )
            {
                if ( directory_temp.direct [i].d_ino == 0 ) {
                    directory_temp.size = i;
                    break;
                }
            }
            iput ( temp_inode );
        }

        unsigned int di_ith, di_ino;
        struct inode * inode;
        di_ino = search_directory_or_file_id_by_name ( directory_temp, dest_filename );//是否已经存在文件或者文件夹
        if ( di_ino != NOTFOUND )	/* already existed */
        {
            cout << "dest have exist, please retry" << endl;
        }
        else /* not existed before */
        {
            directory_temp = directory;
            int i;
            for ( i = 0; i < dirs.size ( ) - 1; i++ )
            {
                string temp_dir = dirs [i];
                dest_dir_id = temp_inode_id = search_directory_or_file_id_by_name ( directory_temp, temp_dir.c_str ( ) );
                if ( temp_inode_id == NOTFOUND ) {
                    cout << "dest dirctory not exist" << endl;
                    return;
                }
                temp_inode = iget ( temp_inode_id );
                if ( !access ( user_id, temp_inode, users [user_id].u_default_mode ) )
                {
                    printf ( "\nhas not access to the directory %s\n", temp_dir.c_str ( ) );
                    iput ( temp_inode );
                    return;
                }
                /* pack the current directory */
                fseek ( fd, DATASTART + BLOCKSIZ*temp_inode->di_addr [0], SEEK_SET );
                fread ( directory_temp.direct, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct ), fd );
                for ( int i = 0; i < MaxNumberOfDirOrFileInADirectory; i++ )
                {
                    if ( directory_temp.direct [i].d_ino == 0 ) {
                        directory_temp.size = i;
                        break;
                    }
                }
                iput ( temp_inode );
            }
            int pos = create_file ( directory_temp, user_id, dest_filename, DEFAULTMODE );
            dest_inode = iget ( directory_temp.direct [pos].d_ino );
            int handle = open_file ( user_id, sourch_filename, READ_AB );
            sourch_inode = sys_ofile [users [user_id].u_ofile [handle]].f_inode;
            int blocks = sourch_inode->di_size / BLOCKSIZ;
            if ( ( sourch_inode->di_size %BLOCKSIZ ) != 0 ) {
                blocks++;
            }
            //复制文件
            char buffer [BLOCKSIZ];
            fseek ( fd, DATASTART + sourch_inode->di_addr [0] * BLOCKSIZ, SEEK_SET );
            fread ( buffer, 1, BLOCKSIZ, fd );
            fseek ( fd, DATASTART + dest_inode->di_addr [0]* BLOCKSIZ, SEEK_SET );
            fwrite ( buffer, 1, BLOCKSIZ, fd );
            for (  i = 1; i < blocks; i++ )
            {
                int new_block = balloc ( );
                dest_inode->di_addr [i] = new_block;
                fseek ( fd, DATASTART + sourch_inode->di_addr [i] * BLOCKSIZ, SEEK_SET );
                fread ( buffer, 1, BLOCKSIZ, fd );
                fseek ( fd, DATASTART + dest_inode->di_addr [i], SEEK_SET );
                fwrite ( buffer, 1, BLOCKSIZ, fd );
            }
            dest_inode->di_size = sourch_inode->di_size;
            iput ( dest_inode );

            temp_inode = iget ( temp_inode_id );
            fseek ( fd, DATASTART + BLOCKSIZ*temp_inode->di_addr [0], SEEK_SET );
            fwrite ( directory_temp.direct, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct ), fd );
            iput ( temp_inode );
            cout << "copy ok!" << endl;
            return;
        }
    }
}
void move_file ( const char * sourch_filename, string dest )
{
    struct inode *sourch_inode, *dest_inode, *temp_inode = nullptr;
    int sourch_inode_id, dest_dir_id, temp_inode_id;
    struct dir directory_temp;
    deque<string> dirs;
    split ( dest, "/", dirs );
    sourch_inode_id = search_directory_or_file_id_by_name ( directory, sourch_filename );
    if ( sourch_inode_id == NOTFOUND ) {
        cout << "file not exist!" << endl;
        return;
    }

    string dest_filename_string = dirs.back ( );
    char *dest_filename;
    int len = dest_filename_string.length ( );
    dest_filename = ( char * ) malloc ( ( len + 1 ) * sizeof ( char ) );
    dest_filename_string.copy ( dest_filename, len, 0 );
    *( dest_filename + len ) = '\0';
    if ( dirs.size ( ) == 1 )
    {//移动到当前目录，相当于重命名

        int i = find_file_or_dirctory_pos_in_directory ( directory, sourch_filename );
        strcpy_s ( directory.direct [i].d_name, dest_filename );
        cout << "move ok!" << endl;
    }
    else
    {//移动到其他文件夹
        directory_temp = directory;
        int i;
        for ( i = 0; i < dirs.size ( ) - 1; i++ )
        {
            string temp_dir = dirs [i];
            dest_dir_id = temp_inode_id = search_directory_or_file_id_by_name ( directory_temp, temp_dir.c_str ( ) );
            if ( temp_inode_id == NOTFOUND ) {
                cout << "dest dirctory not exist" << endl;
                return;
            }
            temp_inode = iget ( temp_inode_id );
            if ( !access ( user_id, temp_inode, users [user_id].u_default_mode ) )
            {
                printf ( "\nhas not access to the directory %s\n", temp_dir.c_str ( ) );
                iput ( temp_inode );
                return;
            }
            /* pack the current directory */
            fseek ( fd, DATASTART + BLOCKSIZ*temp_inode->di_addr [0], SEEK_SET );
            fread ( directory_temp.direct, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct ), fd );
            for ( int i = 0; i < MaxNumberOfDirOrFileInADirectory; i++ )
            {
                if ( directory_temp.direct [i].d_ino == 0 ) {
                    directory_temp.size = i;
                    break;
                }
            }
            iput ( temp_inode );
        }


        unsigned int di_ith, di_ino;
        struct inode * inode;
        di_ino = search_directory_or_file_id_by_name ( directory_temp, dest_filename );//是否已经存在文件或者文件夹
        if ( di_ino != NOTFOUND )	/* already existed */
        {
            cout << "dest have exist, please retry" << endl;
        }
        else /* not existed before */
        {

            int pos = put_the_name_to_current_dir ( directory_temp, dest_filename );
            directory_temp.direct [pos].d_ino = sourch_inode_id;
            directory_temp.size++;
            temp_inode = iget ( temp_inode_id );
            fseek ( fd, DATASTART + BLOCKSIZ*temp_inode->di_addr [0], SEEK_SET );
            fwrite ( directory_temp.direct, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct ), fd );
            iput ( temp_inode );

            //将文件信息在本文件夹中删除
            for ( i = 0; i < directory.size; i++ )
            {
                if ( directory.direct [i].d_ino == sourch_inode_id )
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
            cout << "move success!" << endl;
            return;
        }
    }
}

