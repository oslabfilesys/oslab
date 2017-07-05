//
// Created by xiaochao on 17-7-5.
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "user_reg_and_login.h"

User* login_user(char *username, char *password) {
    FILE *fp = open_file_for_read();
    User *user = nullptr;
    while ((user = read_a_user_from_file(fp)) != nullptr){
        if (verify_user(username, password, user)){
            break;
        } else{
            free(user);
        }
    }
    return user;
}

void _register_user() {
    char username[20], password[20], password_1[20];
    printf("please input your username\n>");
    scanf("%s", username);
    if (exist_user(username)) {
        printf("the user %s is already exist!", username);
    } else {
        printf("please input your password:\n>");
        scanf("%s", password);
        printf("please input your password again:\n>");
        scanf("%s", password_1);
        if (strcmp(password, password) != 0) {
            printf("password not match, please retry!\n");
            return;
        } else {
            User user;
            strcpy(user.username, username);
            strcpy(user.password, password);
            write_a_user_to_file(&user);
        }
    }
}

User* read_a_user_from_file(FILE *file){
    User * user = (User*) malloc(sizeof(User));
    fread(user, sizeof(User), 1, file);
    return user;

}

FILE* open_file_for_read(){
    FILE *fp = nullptr;
    if((fp = fopen(FILENAME, "rb")) == nullptr){
        fp = fopen(FILENAME, "w");
    }
    return fp;
}

FILE* open_file_for_write(){
    FILE *fp = nullptr;
    fp = fopen(FILENAME, "ab+");
    return fp;
}

void write_a_user_to_file(User *user){
    FILE *fp = open_file_for_write();
    fseek(fp, 0, SEEK_END);
    fwrite(user, sizeof(User), 1, fp);
}

bool verify_user(char *username, char *password, User *user) {
    return (strcmp(username, user->username) == 0) && (strcmp(password, user->password) == 0);
}

bool exist_user(char* username){
    FILE *fp = open_file_for_read();
    bool is_exist_user = false;
    User *user = nullptr;
    while ((user = read_a_user_from_file(fp)) != nullptr){
        if (strcmp(user->username, username) == 0){
            free(user);
            is_exist_user = true;
            break;
        } else{
            free(user);
        }
    }
    return is_exist_user;
}

