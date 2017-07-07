//
// Created by xiaochao on 17-7-4.
//
#include"user_reg_and_login.h"
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
//todo 文件共享
bool test_command(const char* command0, const char *command1);
int parse_first_command(char* first_command);
char* get_one_arg(char * next_command);


void help();
void ls(char * next_command);
void create(char * next_command);
void _delete(char * next_command);
void open(char * next_command);
void close(char * next_command);
void read(char * next_command);
void write(char * next_command);
void login(char * next_command);
void _register(char * next_command);
void logout(char * next_command);
void rm(char * next_command);
void mv(char * next_command);
void cp(char * next_command);
#endif //OSLAB_COMMAND_PARSE_H
