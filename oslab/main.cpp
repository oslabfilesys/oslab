#include<stdio.h>
#include <cstring>
#include"FILESYS.h"

#include"command_parse.h"
#include"format.h"
#include"access.h"
#include"ballocfre.h"
#include"creat_and_delete.h"
#include"iallfre.h"
#include"igetput.h"
#include"open_and_close.h"
#include"read_and_write.h"
#include"search_and_dir.h"
#include <conio.h>




//extern User current_user;
struct hinode hash_index_node [NumOfHashIndexNode];
struct dir directory;//当前目录，包含当前目录下的目录和文件
struct file sys_ofile[SYSOPENFILE];
struct filsys file_system;
struct pwd _pwd[PWDNUM];
struct user users[USERNUM];
FILE * fd;
struct inode * cur_path_inode;
int user_id;


void split ( const string& src, const string& separator, deque<string>& dest )
{
    string str = src;
    string substring;
    string::size_type start = 0, index;

    do
    {
        index = str.find_first_of ( separator, start );
        if ( index != string::npos )
        {
            substring = str.substr ( start, index - start );
            dest.push_back ( substring );
            start = str.find_first_not_of ( separator, index );
            if ( start == string::npos ) return;
        }
    } while ( index != string::npos );

    //the last token
    substring = str.substr ( start );
    dest.push_back ( substring );
}


int main() {
    user_id = NOTLOGIN;
    if ( fopen_s ( &fd, "filesystem", "rb+" ) != 0 )
    {
        printf ( "\nfilesystem have not format,please wait!\n" );
        format ( );
    } 



    install ( );

    string command;
    deque<string> commands;
	printf("--------------welcome to xxx file system-----------------\n");
    printf("input help or command  help can get help\n");
    while (true){
        command.empty ( );
        commands.clear ( );
        printf(">");

        getline ( cin, command );
        split ( command, " ", commands );
        string first_command ;
        if ( commands.size ( ) != 0 )
        {
            first_command = commands [0];
            commands.pop_front ( );
        }

		if (first_command.length() != 0)
		{
			switch (parse_first_command( first_command )) {
			case HELP:
				help();
				break;
			case LS:
				ls( commands );
				break;
			case CREATE:
				_create( commands );
				break;
			case DELETE:
				_delete( commands );
				break;
			case READ:
				read( commands );
				break;
			case WRITE:
				write( commands );
				break;
			case LOGIN:
				login( commands );
				break;
			case REGISTER:
				_register( commands );
				break;
			case LOGOUT:
				logout( commands );
				break;
			case MV:
				mv( commands );
				break;
			case CP:
				cp( commands );
				break;
            case QUIT:
                halt ( );
                break;
            case FORMAT:
                printf ( "\nDo you want to format the disk \n" );
                if ( _getch ( ) == 'y' ){
                    format ( );
                    install ( );
                }
                break;
            case MKDIR:
                _mkdir ( commands );
                break;
            case CHDIR:
                _chdir ( commands );
                break;
			default:
				printf("unknown command,please retry or input help to get help\n");
			}
		}

    }
    
    system ("pause");
    return 0;
}
