#include<stdio.h>
#include <cstring>
#include"FILESYS.h"

#include"format.h"
#include"access.h"
#include"ballocfre.h"
#include"creat_and_delete.h"
#include"iallfre.h"
#include"igetput.h"
#include"open_and_close.h"
#include"read_and_write.h"
#include"search_and_dir.h"




//extern User current_user;
struct hinode hash_index_node [NumOfHashIndexNode];
struct dir directory;
struct file sys_ofile[SYSOPENFILE];
struct filsys file_system;
struct pwd _pwd[PWDNUM];
struct user users[USERNUM];
FILE * fd;
struct inode * cur_path_inode;
int user_id, file_block;


int main() {
    unsigned short ab_fd1, ab_fd2, ab_fd3, ab_fd4;
    unsigned short bhy_fd1;
    char * buf;
    int ccc = sizeof ( unsigned int );
    printf ( "\nDo you want to format the disk \n" );
    if ( getchar( ) == 'y' )
        format ( );


    install ( );
    for ( int i = 0; i < 7; i++ )
    {
        balloc ( );
    }
    balloc ( );
    balloc ( );
    balloc ( );
    bfree ( 11 );
    bfree ( 10 );
    balloc ( );
    //printf ( "\nCommand : dir  \n" );
    //\

    //    _dir ( );

    //login ( 2118, "abcd" );
    //user_id = 0;
    //mkdir ( "a2118" );
    //chdir ( "a2118" );
    //ab_fd1 = create_file ( user_id, "ab_file0.c", 01777 );
    //buf = ( char * ) malloc ( BLOCKSIZ * 6 + 5 );
    //write_file ( ab_fd1, buf, BLOCKSIZ * 6 + 5 );
    //close_file ( user_id, ab_fd1 );
    //free ( buf );

    //mkdir ( "subdir" );
    //chdir ( "subdir" );
    //ab_fd2 = create_file ( user_id, "file1.c", 01777 );
    //buf = ( char * ) malloc ( BLOCKSIZ * 4 + 20 );
    //write_file ( ab_fd2, buf, BLOCKSIZ * 4 + 20 );
    //close_file ( user_id, ab_fd2 );
    //free ( buf );

    //chdir ( ".." );
    //ab_fd3 = create_file ( user_id, "_file2.c", 01777 );
    //buf = ( char * ) malloc ( BLOCKSIZ * 10 + 255 );
    //write_file ( ab_fd3, buf, BLOCKSIZ * 3 + 255 );
    //close_file ( user_id, ab_fd3 );
    //free ( buf );

    //delete_file( "ab_file0.c" );
    //ab_fd4 = create_file ( user_id, "ab_file3.c", 01777 );
    //buf = ( char * ) malloc ( BLOCKSIZ * 8 + 300 );
    //write_file ( ab_fd4, buf, BLOCKSIZ * 8 + 300 );
    //close_file ( user_id, ab_fd4 );
    //free ( buf );

    //ab_fd3 = open_file ( user_id, "ab_file2.c", FAPPEND );
    //buf = ( char * ) malloc ( BLOCKSIZ * 3 + 100 );
    //write_file ( ab_fd3, buf, BLOCKSIZ * 3 + 100 );
    //close_file (user_id,  ab_fd3 );
    //free ( buf );

    //_dir ( );
    //chdir ( ".." );
    //logout ( user_id);
 //   halt ( );

    /*
	printf("--------------welcome to xxx file system-----------------\n");
    printf("input help or command  help can get help\n");
    while (true){
        char command[100];
        printf("%s>", current_user.username);
        gets_s(command);
        char* first_command, *next_command;
        first_command = strtok_s(command, " ", &next_command);
		if (first_command != nullptr)
		{
			switch (parse_first_command(first_command)) {
			case HELP:
				help();
				break;
			case LS:
				ls(next_command);
				break;
			case CREATE:
				create(next_command);
				break;
			case DELETE:
				_delete(next_command);
				break;
			case OPEN:
				open(next_command);
				break;
			case CLOSE:
				close(next_command);
				break;
			case READ:
				read(next_command);
				break;
			case WRITE:
				write(next_command);
				break;
			case LOGIN:
				login(next_command);
				break;
			case REGISTER:
				_register(next_command);
				break;
			case LOGOUT:
				logout(next_command);
				break;
			case MV:
				mv(next_command);
				break;
			case CP:
				cp(next_command);
				break;
			default:
				printf("unknown command,please retry or input help to get help\n");
			}
		}

    }
    */
    return 0;
}
