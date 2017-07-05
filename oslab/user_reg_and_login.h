//
// Created by xiaochao on 17-7-5.
//

#ifndef OSLAB_USER_REG_AND_LOGIN_H
#define OSLAB_USER_REG_AND_LOGIN_H

#define FILENAME "user.dat"

typedef struct {
    char username[20];
    char password[20];
}User;
User* login_user(char *username, char* password);
void _register_user();

User* read_a_user_from_file(FILE *file);
FILE* open_file_for_read();
FILE* open_file_for_write();
void write_a_user_to_file(User *user);
bool verify_user(char* username, char* password, User* user);
bool exist_user(char* username);
#endif //OSLAB_USER_REG_AND_LOGIN_H
