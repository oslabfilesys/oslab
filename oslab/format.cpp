#include"format.h"

void format()
{
	struct inode * inode;
	struct direct dir_buf[BLOCKSIZ / (DIRSIZ + 2)];
//	struct pwd passwd[BLOCKSIZ / (PWDSIZ + 4)];
	struct filsys filsys;
	unsigned int block_buf[BLOCKSIZ / sizeof(int)];
	char * buf;
	char * empty;
	int i,j,k;
	/*	creat the file system file */
    fopen_s ( &fd, "filesystem", "w+b" );
	buf = (char *)malloc((DINODEBLK + FILEBLK + 2) * BLOCKSIZ * sizeof(char));
	if (fd == NULL)
	{
		printf("\nfile system file creat failed! \n");
		exit (0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(buf, 1,( DINODEBLK + FILEBLK + 2 ) * BLOCKSIZ * sizeof ( char ),  fd);
	free(buf);
	//初始化硬盘
    _pwd [0].p_uid = 2116; _pwd [0].p_gid = 03;
    strcpy_s ( _pwd [0].password, "dddd" );
    _pwd [1].p_uid = 2117; _pwd [1].p_gid = 03;
    strcpy_s ( _pwd [1].password, "bbbb" );
    _pwd [2].p_uid = 2118; _pwd [2].p_gid = 04;
    strcpy_s ( _pwd [2].password, "abcd" );
    _pwd [3].p_uid = 2119; _pwd [3].p_gid = 04;
    strcpy_s ( _pwd [3].password, "cccc" );
    _pwd [4].p_uid = 2220; _pwd [4].p_gid = 05;
    strcpy_s ( _pwd [4].password, "eeee" );
	/*	1.creat the main directory and its sub dir etc and the file password */
	inode = iget(0);	/* 0 empty dinode id */
	inode->di_number = 1;//关联文件数设为1    
	inode->di_mode = DIEMPTY;//权限设为空权限
	iput(inode);//回收inode结点
	inode = iget(1);    /* 1 main dir id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3 * (DIRSIZ + 2);//    inode->di_size = 3*(DIRSIZ + 4);
	inode->di_addr[0] = 0;    /* block 0tfl is used by the main directory */
	strcpy_s(dir_buf[0].d_name, "..");//父目录
	dir_buf[0].d_ino = 1;//磁盘inode结点标号为1
	strcpy_s(dir_buf[1].d_name, ".");//根目录
	dir_buf[1].d_ino = 1;
	strcpy_s(dir_buf[2].d_name, "etc");//子目录etc目录
	dir_buf[2].d_ino = 2;

	fseek(fd, DATASTART, SEEK_SET);
	fwrite(dir_buf, 1,3 * ( DIRSIZ + 2 ), fd);
   //dir_buf中的前3个元素拷贝到磁盘数据区的第1个数据块
	iput(inode);

	/*fseek(fd, 1056, SEEK_SET);
	fread(inode, DINODESIZ, 1, fd);
	inode = iget(1);
	iput(inode);*/
	inode = iget(2);/* 2 etc dir id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3 * (DIRSIZ + 2);
	inode->di_addr[0] = 1;    /* block 0# is used by the etc */

	strcpy_s(dir_buf[0].d_name, "..");
	dir_buf[0].d_ino = 1;
	strcpy_s(dir_buf[1].d_name, "..");
	dir_buf[1].d_ino = 2;
	strcpy_s(dir_buf[2].d_name, "password");//子目录是password
	dir_buf[2].d_ino = 3;
	fseek(fd, DATASTART + BLOCKSIZ * 1, SEEK_SET);
	fwrite(dir_buf,1, 3 * ( DIRSIZ + 2 ), fd);
	//dir_buf中的前3个元素拷贝到磁盘数据区的第2个数据块
	iput(inode);

	inode = iget(3);    /* 3 password id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIFILE;
	inode->di_size = BLOCKSIZ;
	inode->di_addr[0] = 2;

	for (i = 5; i<PWDNUM; i++) {//设置其余用户的属性
		_pwd[i].p_uid = 0;
		_pwd[i].p_gid = 0;
		strcpy(_pwd[i].password, "            ");  // 密码为空
	}

	fseek(fd, DATASTART + 2 * BLOCKSIZ, SEEK_SET);
	fwrite( _pwd, BLOCKSIZ, 1, fd);
	iput(inode);
	/*	2. initialize the superblock */
	filsys.s_isize = DINODEBLK;
	filsys.s_fsize = FILEBLK;
	filsys.s_ninode = DINODEBLK * BLOCKSIZ / DINODESIZ - 4;
	filsys.s_nfree = FILEBLK - 3;
	for (i = 0; i<NICINOD; i++)
	{
		/*	begin with 4, 0,1.2,3, is used by main, etc, password */
		filsys.s_inode[i] = 4 + i;
	}
	filsys.s_pinode = 0;
	filsys.s_rinode = NICINOD + 4;
	for(i=NICFREE+2;i<FILEBLK;i+=50)
	{
		for (j = 0;j < NICFREE;j++) {
			block_buf[NICFREE - 1 - j] = i - j;
		}
		fseek(fd, DATASTART + BLOCKSIZ*(i - 49), SEEK_SET);
		fwrite(block_buf, 1, BLOCKSIZ, fd);
	}//当i=502之后，完成文件块502-453的写入；
	//之后文件块512-503不能进行，需要特殊处理
	for (i = 503;i < 512;i++)block_buf[i - 503] = i;
	fseek(fd, DATASTART + BLOCKSIZ * 503, SEEK_SET);
	fwrite(block_buf, 1, BLOCKSIZ, fd);//完成块定位
	for (i = 0;i < NICFREE;i++) {
		filsys.s_free[i] = i + 3;//DATASTART的第一个BLOCK作为MAIN DIRECTORY
								 //第二个BLOCK作为etc目录 //第三个BLOCK作为password文 //故此i要加3
	}
	j = 1;
	filsys.s_pfree = NICFREE - j;
	filsys.s_pinode = 0;
	fseek(fd, BLOCKSIZ, SEEK_SET);
	fwrite(&filsys, 1, sizeof(struct filsys), fd);
	fclose(fd);
}

void install ( )
{
    fclose ( fd );
    int i, j;
    fopen_s ( &fd, "filesystem", "rb+");
    /*	1. read the filsys from the superblock */
    fseek ( fd, BLOCKSIZ, SEEK_SET );
    fread ( &file_system, sizeof ( struct filsys ), 1, fd );
    /*	2. initialize the mode hash chain */
    for ( i = 0; i<NHINO; i++ )
    {
        h_inode [i].i_forw = NULL;
    }
    /*	3. initjalize the sys-ofile */
    for ( i = 0; i<SYSOPENFILE; i++ )
    {
        sys_ofile [i].f_count = 0;
        sys_ofile [i].f_inode = NULL;
    }
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
    directory.size = cur_path_inode->di_size / ( DIRSIZ + 2 );
    for ( i = 0; i<DIRNUM; i++ )
    {
        strcpy_s ( directory.direct [i].d_name, "    " );
        directory.direct [i].d_ino = 0;
    }
    for ( i = 0; i<directory.size / ( BLOCKSIZ / ( DIRSIZ + 2 ) ); i++ )
    {
        fseek ( fd, DATASTART + BLOCKSIZ * cur_path_inode->di_addr [i], SEEK_SET );
        fread ( &directory.direct [( BLOCKSIZ / ( DIRSIZ + 2 ) ) * i], 1, BLOCKSIZ, fd );
    }
    fseek ( fd, DATASTART + BLOCKSIZ * cur_path_inode->di_addr [i], SEEK_SET );
    fread ( &directory.direct [( BLOCKSIZ ) / ( DIRSIZ + 2 ) * i], 1, cur_path_inode->di_size % BLOCKSIZ, fd );
}