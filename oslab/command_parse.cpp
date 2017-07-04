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

char* get_one_arg(){
    return strtok(nullptr, " ");

}


void help() {
    printf("欢迎使用本文件系统，系统命令如下：\n");
    printf("login 用户名 密码：登入用户\n");
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
}

void ls() {
    char *target_directory = get_one_arg();
    if (target_directory == nullptr){
        printf("列出当前目录文件\n");
    } else{
        printf("列出目录 %s 的文件\n", target_directory);
    }
}

void create() {
    char *arg = get_one_arg();
    if (arg == nullptr){
        printf("输入的指令有误， 请检查或者输入create help 获取帮助\n");
    } else if (test_command(arg, "help")){
        printf("create 文件名:创建指定文件\n");
    } else{
        char* target_file = arg;
        printf("尝试创建 %s 文件\n", target_file);

    }
}

void _delete() {
    char *arg0 = get_one_arg();
    char *arg1 = get_one_arg();
    if (arg0 == nullptr || (test_command(arg0, "all") && arg1 == nullptr))
        printf("输入的指令有误， 请检查或者输入create help 获取帮助\n");
    else if (test_command(arg0, "help")){
        printf("delete [参数] 文件名或文件夹:删除指定文件或目录\n");
        printf("参数：all 删除指定文件或者目录（无论里面是否有文件）");
    } else if (test_command(arg0, "all")){
        char * target_file_or_directory = arg1;
        printf("删除指定文件:%s \n", target_file_or_directory);
    }

}

void open() {
    char *arg = get_one_arg();
    if (arg == nullptr){
        printf("输入的指令有误， 请检查或者输入open help 获取帮助\n");
    } else if (test_command(arg, "help")){
        printf("open 文件名:打开指定文件\n");
    } else{
        char* target_file = arg;
        printf("尝试打开 %s 文件\n", target_file);

    }
}

void close() {
    char *arg = get_one_arg();
    if (arg == nullptr){
        printf("输入的指令有误， 请检查或者输入close help 获取帮助\n");
    } else if (test_command(arg, "help")){
        printf("close 文件名:打开指定文件\n");
    } else{
        char* target_file = arg;
        printf("尝试关闭 %s 文件\n", target_file);

    }
}

void read() {
    char *arg = get_one_arg();
    if (arg == nullptr){
        printf("输入的指令有误， 请检查或者输入read help 获取帮助\n");
    } else if (test_command(arg, "help")){
        printf("read 文件名:读取指定文件\n");
    } else{
        char* target_file = arg;
        printf("尝试读取 %s 文件\n", target_file);

    }
}

void write() {
    char *arg = get_one_arg();
    if (arg == nullptr){
        printf("输入的指令有误， 请检查或者输入write help 获取帮助\n");
    } else if (test_command(arg, "help")){
        printf("write 文件名:写入指定文件\n");
    } else{
        char* target_file = arg;
        printf("尝试写入 %s 文件\n", target_file);

    }
}

void login() {
    char *arg0 = get_one_arg();
    char *arg1 = get_one_arg();
    if (arg0 == nullptr  || (!test_command(arg0, "help") && arg1 == nullptr ))
        printf("输入的指令有误， 请检查或者输入login help 获取帮助\n");
    else if (test_command(arg0, "help")){
        printf("login [用户名] [密码]：登入用户\n");
    } else {
        char* username = arg0;
        char* password = arg1;
        printf("尝试登入用户%s\n", username);
    }
}

void _register() {

}

void logout() {

}

void rm() {

}

void mv() {

}

void cp() {

}
