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
    fopen_s ( &fd, "filesystem", "wb+" );
	buf = (char *)malloc((DINODEBLK + FILEBLK + 2) * BLOCKSIZ * sizeof(char));
	if (fd == NULL)
	{
		printf("\nfile system file creat failed! \n");
		exit (0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(buf, ( DINODEBLK + FILEBLK + 2 ) * BLOCKSIZ * sizeof ( char ), 1, fd);
	/*0.initialize the passwd */
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
	inode->di_mode = DIEMPTY;
	iput(inode);
	inode = iget(1);    /* 1 main dir id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3 * (DIRSIZ + 2);
	inode->di_addr[0] = 0;    /* block 0tfl is used by the main directory */
	strcpy_s(dir_buf[0].d_name, "..");
	dir_buf[0].d_ino = 1;
	strcpy_s(dir_buf[1].d_name, ".");
	dir_buf[1].d_ino = 1;
	strcpy_s(dir_buf[2].d_name, "etc");
	dir_buf[2].d_ino = 2;
	fseek(fd, DATASTART, SEEK_SET);
	fwrite(dir_buf, 3 * ( DIRSIZ + 2 ), 1, fd);
	iput(inode);
	inode = iget(2);/* 2 etc dir id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3 * (DIRSIZ + 2);
	inode->di_addr[0] = 0;    /* block 0# is used by the etc */
	strcpy_s(dir_buf[0].d_name, "..");
	dir_buf[0].d_ino = 1;
	strcpy_s(dir_buf[1].d_name, "..");
	dir_buf[1].d_ino = 2;
	strcpy_s(dir_buf[2].d_name, "password");
	dir_buf[2].d_ino = 3;
	fseek(fd, DATASTART + BLOCKSIZ * 1, SEEK_SET);
	fwrite(dir_buf, 3 * ( DIRSIZ + 2 ), 1, fd);
	iput(inode);
	inode = iget(3);    /* 3 password id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIFILE;
	inode->di_size = BLOCKSIZ;
	inode->di_addr[0] = 2;

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
	/*FILEBLK+1 is a flag of end */
	block_buf[NICFREE - 1] = FILEBLK + 1;
	for (i = 0;i<NICFREE - 1;i++)
		block_buf[NICFREE - 2 - i] = FILEBLK - i;
	fseek(fd, DATASTART + BLOCKSIZ * (FILEBLK - NICFREE - 1), SEEK_SET);
	fwrite(block_buf, BLOCKSIZ, 1, fd);
	for (i = FILEBLK - NICFREE - 1; i>2; i -= NICFREE)
	{
		for (j = 0;j<NICFREE;j++)
		{
			block_buf[j] = i - j;
		}
		block_buf[j] = 50;
		fseek(fd, DATASTART + BLOCKSIZ * (i - 1), SEEK_SET);
		fwrite(block_buf, 1, BLOCKSIZ, fd);
	}
	j = i + NICFREE;
	for (i = j; i>2;i--)
	{
		filsys.s_free[NICFREE - 1 + i - j] = i;
	}
	filsys.s_pfree = NICFREE - 1-j+3;
	filsys.s_pinode = 0;
	fseek(fd, BLOCKSIZ, SEEK_SET);
	fwrite(&filsys, sizeof ( struct filsys ), 1, fd);
    fclose ( fd );

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