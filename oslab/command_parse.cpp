//
// Created by xiaochao on 17-7-4.
//

#include <cstring>
#include <cstdio>
#include "command_parse.h"
bool test_command(const char* command0, const char *command1){
    return strcmp(command0, command1)==0;
}

int parse_first_command(char* first_command){
    if (test_command(first_command, "help"))
        return HELP;
    else if (test_command(first_command, "ls"))
        return LS;
    else if (test_command(first_command, "create"))
        return CREATE;
    else if (test_command(first_command, "delete"))
        return DELETE;
    else if (test_command(first_command, "open"))
        return OPEN;
    else if (test_command(first_command, "close"))
        return CLOSE;
    else if (test_command(first_command, "read"))
        return READ;
    else if (test_command(first_command, "write"))
        return WRITE;
    else if (test_command(first_command, "login"))
        return LOGIN;
    else if (test_command(first_command, "register"))
        return REGISTER;
    else if (test_command(first_command, "logout"))
        return LOGOUT;
    else if (test_command(first_command, "rm"))
        return RM;
    else if (test_command(first_command, "mv"))
        return MV;
    else if (test_command(first_command, "cp"))
        return CP;
    else
        return UNKNOWN_COMMAND;
}


void help() {
    printf("欢迎使用本文件系统，系统命令如下：\n");
    printf("login [用户名] [密码]：登入用户\n");
    printf("register :注册（会提示输入信息）\n");
    printf("logout 登出当前用户\n");
    printf("ls [文件目录（相对目录或者绝对目录）]: 列出本目录或者指定目录的文件信息\n");
    printf("create 文件名:创建指定文件\n");
    printf("delete [参数列表] 文件夹/文件名： 删除指定文件夹或者文件\n");
    printf("open 文件名： 打开指定文件\n");
    printf("close 文件名：关闭指定文件\n");
    printf("read 文件名：读取指定文件\n");
    printf("write 文件名：写入指定文件\n");
    printf("mv 源文件 目标地点：移动文件\n");
    printf("cp 源文件 目标地点：复制文件\n");
    printf("命令详情请输入对应命令+help\n");
    printf(">");

}

void ls( char *command) {

}

void create( char *command) {

}

void _delete( char *command) {

}

void open( char *command) {

}

void close( char *command) {

}

void read( char *command) {

}

void write( char *command) {

}

void login( char *command) {

}

void _register( char *command) {

}

void logout( char *command) {

}

void rm( char *command) {

}

void mv( char *command) {

}

void cp( char *command) {

}
