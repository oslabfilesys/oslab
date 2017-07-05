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
    while (true){
        char command[100];
        printf(">");
        gets(command);
        char* first_command;
        first_command = strtok(command, " ");
        switch (parse_first_command(first_command)){
            case HELP:
                help();
                break;
            case LS:
                ls();
                break;
            case CREATE:
                create();
                break;
            case DELETE:
                _delete();
                break;
            case OPEN:
                open();
                break;
            case CLOSE:
                close();
                break;
            case READ:
                read();
                break;
            case WRITE:
                write();
                break;
            case LOGIN:
                login();
                break;
            case REGISTER:
                _register();
                break;
            case LOGOUT:
                logout();
                break;
            case MV:
                mv();
                break;
            case CP:
                cp();
                break;
            default:
                printf("未知指令，请重新输入或者输入help获取帮助\n");
        }

    }
    return 0;
}

#pragma clang diagnostic pop