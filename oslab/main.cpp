#include<stdio.h>
#include <cstring>
#include"FILESYS.h"
#include "command_parse.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"




int main() {
    //todo init or load filesystem
	printf("--------------欢迎使用×××文件系统-----------------\n");
    printf("输入 help 或 命令  help 可获得提示\n");
    printf(">");
    while (true){
        char command[100];
        gets(command);
        char* first_command;
        first_command = strtok(command, " ");
        switch (parse_first_command(first_command)){
            case HELP:
                help();
                break;
            case LS:
                ls(command);
                break;
            case CREATE:
                create(command);
                break;
            case DELETE:
                _delete(command);
                break;
            case OPEN:
                open(command);
                break;
            case CLOSE:
                close(command);
                break;
            case READ:
                read(command);
                break;
            case WRITE:
                write(command);
                break;
            case LOGIN:
                login(command);
                break;
            case REGISTER:
                _register(command);
                break;
            case LOGOUT:
                logout(command);
                break;
            case RM:
                rm(command);
                break;
            case MV:
                mv(command);
                break;
            case CP:
                cp(command);
                break;
            default:
                printf("未知指令，请重新输入或者输入help获取帮助\n");
                printf(">");
        }

    }
    return 0;
}

#pragma clang diagnostic pop