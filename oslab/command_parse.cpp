//
// Created by xiaochao on 17-7-4.
//

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "command_parse.h"
#include "user_reg_and_login.h"

User current_user;

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

char* get_one_arg(char * next_command){
    return strtok_s(nullptr, " ", &next_command);

}

void help() {
    printf("welcome to the file system,the command are here:\n");
    printf("login username password:login user\n");
    printf("register :register a new user\n");
    printf("logout :logout current user\n");
    printf("ls [directory]: list the files and directories\n");
    printf("create filename:create the file\n");
    printf("delete [args] directory/filename:  delete the directory or file\n");
    printf("open filenam: open the file\n");
    printf("close filename: close file\n");
    printf("read filename: read the file\n");
    printf("write filename: write the file\n");
    printf("mv source target: move the file\n");
    printf("cp source target: copy the file\n");
    printf("for details can input command+help\n");
}

void ls(char * next_command) {
    char *target_directory = get_one_arg(next_command);
    if (target_directory == nullptr){
        printf("list current directory\n");
    } else{
        printf("list directory %s 's files\n", target_directory);
    }
}

void create(char * next_command) {
    char *arg = get_one_arg(next_command);
    if (arg == nullptr){
        printf("there's something wrong,please check it or input create help to get help\n");
    } else if (test_command(arg, "help")){
        printf("create filename:create the file\n");
    } else{
        char* target_file = arg;
        printf("try to create %s file\n", target_file);

    }
}

void _delete(char * next_command) {
    char *arg0 = strtok_s(nullptr, " ", &next_command);
    char *arg1 = strtok_s(nullptr, " ", &next_command);
    if (arg0 == nullptr || (test_command(arg0, "all") && arg1 == nullptr))
        printf("there's something wrong,please check it or input delete help to get help\n");
    else if (test_command(arg0, "help")){
        printf("delete [args] directory/filename:  delete the directory or file\n");
        printf("arg：all delete all files in the directory and the directory");
    } else if (test_command(arg0, "all")){
        char * target_file_or_directory = arg1;
        printf("delete:%s \n", target_file_or_directory);
    }

}

void open(char * next_command) {
    char *arg = get_one_arg(next_command);
    if (arg == nullptr){
        printf("there's something wrong,please check it or input open help to get help\n");
    } else if (test_command(arg, "help")){
        printf("open filename:open the file\n");
    } else{
        char* target_file = arg;
        printf("try to open the %s file\n", target_file);

    }
}

void close(char * next_command) {
    char *arg = get_one_arg(next_command);
    if (arg == nullptr){
        printf("there's something wrong,please check it or input close help to get help\n");
    } else if (test_command(arg, "help")){
        printf("close filename:close the file\n");
    } else{
        char* target_file = arg;
        printf("try to close %s file\n", target_file);

    }
}

void read(char * next_command) {
    char *arg = get_one_arg(next_command);
    if (arg == nullptr){
        printf("there's something wrong,please check it or input read help to get help\n");
    } else if (test_command(arg, "help")){
        printf("read filename:read the file\n");
    } else{
        char* target_file = arg;
        printf("try to read the %s file\n", target_file);

    }
}

void write(char * next_command) {
    char *arg = get_one_arg(next_command);
    if (arg == nullptr){
        printf("there's something wrong,please check it or input write help to get help\n");
    } else if (test_command(arg, "help")){
        printf("write filename:write file\n");
    } else{
        char* target_file = arg;
        printf("try to write the %s file\n", target_file);

    }
}

void login(char * next_command) {
    char *arg0 = strtok_s(nullptr, " ", &next_command);
    char *arg1 = strtok_s(nullptr, " ", &next_command);
    if (arg0 == nullptr  || (!test_command(arg0, "help") && arg1 == nullptr ))
        printf("there's something wrong,please check it or input login help to get help\n");
    else if (test_command(arg0, "help")){
        printf("login username password：login the user\n");
    } else {
		User *user;
        char* username = arg0;
        char* password = arg1;
        user = login_user(username, password);
		if (user == nullptr)
		{
			printf_s("login failed, please retry.\n");
		}
		else
		{
			strcpy_s(current_user.username, user->username);
			printf_s("login ok!\nwelcome %s!\n", current_user.username);
			free(user);
		}
    }
}

void _register(char * next_command) {
    char* arg0 = get_one_arg(next_command);
    if (arg0 != nullptr && test_command(arg0, "help")){
        printf("register：register the user\n");
    } else{
        _register_user();
    }
}

void logout(char * next_command) {
    char* arg0 = get_one_arg(next_command);
    if (arg0!= nullptr && test_command(arg0, "help")){
        printf("logout：logout current user\n");
    } else{
		if (strcmp(current_user.username, "") == 0)
		{
			printf_s("no user is login!\n");
		}
		else
		{
			strcpy_s(current_user.username, "");
			printf_s("logout ok!\n");
		}

    }
}

void mv(char * next_command) {
    char *arg0 = strtok_s(nullptr, " ", &next_command);
    char *arg1 = strtok_s(nullptr, " ", &next_command);
    if (arg0 == nullptr  || (!test_command(arg0, "help") && arg1 == nullptr ))
        printf("there's something wrong,please check it or input mv help to get help\n");
    else if (test_command(arg0, "help")){
        printf("mv source target：move the file\n");
    } else {
        char* source = arg0;
        char* target = arg1;
        printf("try to move the file %s\n", source);
    }
}

void cp(char * next_command) {
    char *arg0 = strtok_s(nullptr, " ", &next_command);
    char *arg1 = strtok_s(nullptr, " ", &next_command);
    if (arg0 == nullptr  || (!test_command(arg0, "help") && arg1 == nullptr ))
        printf("there's something wrong,please check it or input cp help to get help\n");
    else if (test_command(arg0, "help")){
        printf("cp source target: copy the file\n");
    } else {
        char* source = arg0;
        char* target = arg1;
        printf("try to copy the file %s\n", source);
    }
}
