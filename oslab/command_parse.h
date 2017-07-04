//
// Created by xiaochao on 17-7-4.
//

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
char* get_one_arg();

void help();
void ls();
void create();
void _delete();
void open();
void close();
void read();
void write();
void login();
void _register();
void logout();
void rm();
void mv();
void cp();
#endif //OSLAB_COMMAND_PARSE_H
