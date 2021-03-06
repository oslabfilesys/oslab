#include"format.h"
#include"open_and_close.h"
#include"search_and_dir.h"

void format()
{
    if ( fd  != nullptr )
    {
        fclose ( fd );
    }
	struct inode * inode;
	struct direct dir_buf[MaxNumberOfDirOrFileInADirectory];
//	struct pwd passwd[BLOCKSIZ / (PWDSIZ + 4)];
	struct filsys filsys;
	unsigned int block_buf[BLOCKSIZ / sizeof(int)];//写入空闲快用的缓存数组
	char * buf;
	int i,j;
	/*	creat the file system file */
    fopen_s ( &fd, "filesystem", "w+b" );
    int disk_size =( ( FileMaxBlockNumber + 2 + DiskIndexNodeBlockNumber ) * BLOCKSIZ ) *sizeof ( char );
	buf = (char *)malloc(disk_size);
	if (fd == NULL)
	{
		printf("\nfile system file creat failed! \n");
		exit (0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(buf, 1, disk_size, fd);//初始化磁盘，char大小是1
	free(buf);

    _pwd [0].p_uid = 0001; _pwd [0].p_gid = 03;
    strcpy_s ( _pwd [0].password, "root" );

    for ( int i = 1; i < PWDNUM; i++ )
    {
        _pwd [i].p_uid = 0;
        strcpy_s ( _pwd [i].password, "" );
    }

	/*	1.creat the main directory and its sub dir etc and the file password */
	inode = iget(0);	/* 0 empty disk index node id */
	inode->di_number = 1;//关联文件数设为1    
	inode->di_mode = DIEMPTY;//权限设为空权限
	iput(inode);//把i节点放回文件

    //根目录
	inode = iget(1);    /* 1 main dir id */
	inode->di_number = 1;
	inode->di_mode = DIR_TYPE;
	inode->di_size = 3 * ( 20 );//    inode->di_size = 3*(DIRSIZ + 4);
	inode->di_addr[0] = 0;    /* block 0tfl is used by the main directory */
	strcpy_s(dir_buf[0].d_name, "..");//父目录
	dir_buf[0].d_ino = 1;//磁盘inode结点标号为1
	strcpy_s(dir_buf[1].d_name, ".");//根目录
	dir_buf[1].d_ino = 1;
	strcpy_s(dir_buf[2].d_name, "etc");//子目录etc目录
	dir_buf[2].d_ino = 2;

    for ( int i = 2; i <MaxNumberOfDirOrFileInADirectory; i++ )
    {
        strcpy_s ( dir_buf [i].d_name, "   " );
        dir_buf [i].d_ino = 0;
    }
	fseek(fd, DATASTART, SEEK_SET);
	fwrite(dir_buf, 1, MaxNumberOfDirOrFileInADirectory * sizeof(struct direct), fd);
   //dir_buf中的前3个元素拷贝到磁盘数据区的第1个数据块
	iput(inode);

    //ect目录
	inode = iget(2);/* 2 etc dir id */
	inode->di_number = 1;
	inode->di_mode = DIR_TYPE;
	inode->di_size = 3 * ( 20 );
	inode->di_addr[0] = 1;    /* block 0# is used by the etc */

	strcpy_s(dir_buf[0].d_name, "..");
	dir_buf[0].d_ino = 1;
	strcpy_s(dir_buf[1].d_name, ".");
	dir_buf[1].d_ino = 2;
	strcpy_s(dir_buf[2].d_name, "password");//子目录是password
	dir_buf[2].d_ino = 3;
	fseek(fd, DATASTART + BLOCKSIZ * 1, SEEK_SET);
	fwrite(dir_buf,1, MaxNumberOfDirOrFileInADirectory * sizeof(struct direct), fd);
	//dir_buf中的前3个元素拷贝到磁盘数据区的第2个数据块
	iput(inode);


	inode = iget(3);    /* 3 password id */
	inode->di_number = 1;
	inode->di_mode = FILE_TYPE;
	inode->di_size = BLOCKSIZ;
	inode->di_addr[0] = 2;
	fseek(fd, DATASTART + 2 * BLOCKSIZ, SEEK_SET);
	fwrite( _pwd, sizeof(_pwd), 1, fd);
	iput(inode);

	/*	2. initialize the superblock */
	filsys.s_isize = DiskIndexNodeBlockNumber;//索引占的块数
	filsys.s_fsize = FileMaxBlockNumber;//数据占的块数
	filsys.surplus_nunmber_of_inode = DiskIndexNodeBlockNumber * BLOCKSIZ / DiskIndexNodeSize - 4;//4块被 root的父目录,root，etc,password占用 223
	filsys.s_nfree = FileMaxBlockNumber - 3;//空闲block数：root，etc,password已经占用了三块数据块

	for (i = 0; i<NICINOD; i++)
	{
		/*	begin with 4, 0,1.2,3, is used by main, etc, password */
		filsys.s_inode[i] = 4 + i;
	}//空闲inode堆栈，从第四块开始分配
	filsys.surplus_point_to_inode = 0; // 空闲inode指针，指向s_inode的第0个元素
	filsys.s_rinode = NICINOD + 4;//下一组inode的开始地点
    /*将inode map表放在block中， 从第一组inode开始存， 第五十一位保存下一组保存地点*/
    unsigned char inode_buffer [51];
    for ( int i = NICINOD + 4; i < filsys.surplus_nunmber_of_inode - 50; i+=50 )
    {
        for ( int j = 0; j < 50; j++ )
        {
            inode_buffer [j] = i + j;
        }
        inode_buffer [50] = i + 50;
        fseek ( fd, DiskIndexNodeStart + DiskIndexNodeSize * i, SEEK_SET );//倒数第五十块
        fwrite ( inode_buffer, 1, 51, fd );
    }
    j = filsys.surplus_nunmber_of_inode - i + 50;
    for (  ; j < filsys.surplus_nunmber_of_inode; j++ )
    {
        inode_buffer [j] = i - 50 + j;
    }
    inode_buffer [50] = filsys.surplus_nunmber_of_inode + 1;//结束标志
    fseek ( fd, DiskIndexNodeStart + DiskIndexNodeSize * (i -50), SEEK_SET );//倒数第五十块
    fwrite ( inode_buffer, 1, 51, fd );

    /*将block map表存放在block中：从最后一个block开始存，每个block存放50个block号*/
    /*FILEBLK+1 is a flag of end */
    //
    for ( i = FileMaxBlockNumber - 1; i > 50; i -= NICFREE )//倒数第五十块开始，直到第二块结束（0 1 2被占用）
    {
        for ( j = 0; j < NICFREE; j++ )
        {
            block_buf [j] = i + j - 50 + 1;// 511 + 0 - 50 + 1= ?? 第一块地址！正着来 
        }
        block_buf [j] = FileMaxBlockNumber + 1;//结束标志513，其实没用到这玩意
        fseek ( fd, DATASTART + BLOCKSIZ * (i -50), SEEK_SET );//把东西保存到前一组的最后一块 本组最后一块511 上一组最后一块是-50
        fwrite ( block_buf, 1, BLOCKSIZ, fd );
    }

    j = i ;//处理最后一组，不足50块
    for ( ; i > 2; i-- )
    {
        filsys.s_free [NICFREE + i - j - 1] = i;
    }
    filsys.s_pfree = NICFREE - j - 1 + 3;
    filsys.surplus_point_to_inode = 0;
    fseek ( fd, BLOCKSIZ, SEEK_SET );
    fwrite ( &filsys, 1, sizeof ( struct filsys ), fd );//写到超级块里面
	fclose(fd);
}

void install ( )
{
    struct direct dir_buf [MaxNumberOfDirOrFileInADirectory];
    if ( fd != nullptr )
    {
        fclose ( fd );
    }

    int i, j;
    fopen_s ( &fd, "filesystem", "rb+");
    /*	1. read the filsys from the superblock */
    fseek ( fd, BLOCKSIZ, SEEK_SET );//读取超级块
    fread ( &file_system, sizeof ( struct filsys ), 1, fd );
    /*	2. initialize the mode hash chain */
    for ( i = 0; i<NumOfHashIndexNode; i++ )
    {
        hash_index_node [i].i_forw = NULL;
    }//初始化hash列表

    /*	3. initjalize the sys-ofile */
    for ( i = 0; i<SYSOPENFILE; i++ )
    {
        sys_ofile [i].f_count = 0;
        sys_ofile [i].f_inode = NULL;
    }//系统打开文件表

    /*4. initialize the user */
    for ( i = 0; i<USERNUM; i++ )
    {
        users [i].u_uid = 0;
        users [i].u_gid = 0;
        for ( j = 0; j<NOFILE; j++ )
        {
            users [i].u_ofile [j] = SYSOPENFILE + 1;
        }
    }


    /* 5. read the main directory to initialize the dir */
    cur_path_inode = iget ( 1 );
    directory.size = 2;// . + .. = 2
    //read the main dir from disk
    fseek ( fd, DATASTART + BLOCKSIZ*cur_path_inode->di_addr [0], SEEK_SET );
    fread ( dir_buf, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct ), fd );
    for ( i = 0; i < MaxNumberOfDirOrFileInADirectory; i++ )
    {
        if ( dir_buf [i].d_ino != 0 ) {
            strcpy_s ( directory.direct [i].d_name, dir_buf [i].d_name );
            directory.direct [i].d_ino = dir_buf [i].d_ino;
        }
        else
        {
            directory.size = i;
            break;
        }
    }
    //读取用户和密码信息
    fseek ( fd, DATASTART + 2 * BLOCKSIZ, SEEK_SET );
    fread ( _pwd, sizeof ( _pwd ), 1, fd );

}

void halt ( ) {
    int i, j;
    /* 1.write back the current dir */
    /*	write back the current directory */
    fseek ( fd, DATASTART + BLOCKSIZ*cur_path_inode->di_addr [0], SEEK_SET );
    fwrite ( directory.direct, 1, MaxNumberOfDirOrFileInADirectory * sizeof ( struct direct ), fd );
    iput ( cur_path_inode );
    /* 2.free the u_ofile and sys_ofile and inode */
    for ( i = 0; i<USERNUM; i++ )
    {
        if ( users [i].u_uid != 0 )
        {
            for ( j = 0; j<NOFILE; j++ )
            {
                if ( users [i].u_ofile [j] != SYSOPENFILE + 1 )
                {
                    close_file (i, users [i].u_ofile [j] );
                    users [i].u_ofile [j] = SYSOPENFILE + 1;
                }
            }
        }
    }
    //保存用户和用户密码
    fseek ( fd, DATASTART + 2 * BLOCKSIZ, SEEK_SET );
    fwrite ( _pwd, BLOCKSIZ, 1, fd );

    /*	3.write back the filesys to the disk */
    fseek ( fd, BLOCKSIZ, SEEK_SET );
    fwrite ( &file_system, 1, sizeof ( struct filsys ), fd );
    /* 4. close the file system column */
    fclose ( fd );
    /*5. say GOOD BYE to all the user */
    printf ( "\nGood Bye. See You Next Time. Please turn off the switch\n" );


}