//
// Created by xiaochao on 17-7-4.
//
#include"user_reg_and_login.h"
#include"FILESYS.h"
#ifndef OSLAB_COMMAND_PARSE_H
#define OSLAB_COMMAND_PARSE_H
//下面是一级命令
#define UNKNOWN_COMMAND 0
#define HELP 1
#define LS 2
#define CREATE 3
#define DELETE 4
#define OPEN 5
#define CLOSE 6
#define READ 7
#define WRITE 8
#define LOGIN 9
#define REGISTER 10
#define LOGOUT 11
#define RM 12
#define MV 13
#define CP 14
#define QUIT 15
#define FORMAT 16
#define MKDIR 17
#define CHDIR 18
//todo 文件共享
bool test_command(string command0, string command1);
int parse_first_command( string first_command);
char* get_one_arg(char * next_command);


void help();
void ls( deque<string>&  commands );
void _create( deque<string>&  commands );
void _delete(deque<string>&  commands);
void read(deque<string>&  commands);
void write(deque<string>&  commands);
void login(deque<string>&  commands);
void _register(deque<string>&  commands);
void logout(deque<string>&  commands);
void rm(deque<string>&  commands);
void mv(deque<string>&  commands);
void cp(deque<string>&  commands);
void _mkdir ( deque<string>&  commands );
void _chdir ( deque<string>&  commands );
#endif //OSLAB_COMMAND_PARSE_H
