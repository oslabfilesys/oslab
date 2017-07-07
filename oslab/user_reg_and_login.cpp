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
	if (fp)
	{
		fclose(fp);
	}
    return user;
}

void _register_user() {
    char username[20], password[20], password_1[20];
    printf_s("please input your username\n>");
    gets_s(username);
    if (exist_user(username)) {
        printf_s("the user %s is already exist!\n", username);
    } else {
        printf("please input your password:\n>");
        gets_s(password);
        printf("please input your password again:\n>");
        gets_s(password_1);
        if (strcmp(password, password) != 0) {
            printf("password not match, please retry!\n");
            return;
        } else {
            User user;
            strcpy_s(user.username, username);
            strcpy_s(user.password, password);
            write_a_user_to_file(&user);
			printf_s("reg success! now you can login!\n");
        }
    }
}

User* read_a_user_from_file(FILE *file){
	User * user;
	if (!feof(file))
	{
		user = (User*)malloc(sizeof(User));
		fread(user, sizeof(User), 1, file);
	}
	else
	{
		user = nullptr;
	}

    return user;

}

FILE* open_file_for_read(){
    FILE *fp;
	fopen_s(&fp, FILENAME, "rb");
    if(fp == nullptr){
		fopen_s(&fp, FILENAME, "wb+");
    }
    return fp;
}

FILE* open_file_for_write(){
    FILE *fp ;
	if (fopen_s(&fp, FILENAME, "ab+") != 0) {
		fopen_s(&fp, FILENAME, "w");
	}
    return fp;
}

void write_a_user_to_file(User *user){
    FILE *fp = open_file_for_write();
	User *temp;
	int last_user_id = 1;
	while ((temp = read_a_user_from_file(fp)) != nullptr) {
		last_user_id = temp->uid;
		delete temp;
	}
	user->uid = last_user_id;
    fwrite(user, sizeof(User), 1, fp);
	if (fp)
	{
		fclose(fp);
	}
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
	if (fp)
	{
		fclose(fp);
	}
    return is_exist_user;
}

