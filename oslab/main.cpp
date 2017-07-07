#include<stdio.h>
#include <cstring>
#include"FILESYS.h"
#include "command_parse.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

extern User current_user;

struct hinode hinode[NHINO];
struct dir dir;
struct file sys_ofile[SYSOPENFILE];
struct filsys filsys;
struct pwd pwd[PWDNUM];
struct user user[USERNUM];
FILE * fd;
struct inode * cur_path_inode;
int user_id;


int main() {
    //todo init or load filesystem

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
    return 0;
}

#pragma clang diagnostic pop