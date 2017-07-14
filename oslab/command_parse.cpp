//
// Created by xiaochao on 17-7-4.
//

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "command_parse.h"
#include"search_and_dir.h"
#include"access.h"
#include"creat_and_delete.h"
#include"open_and_close.h"
#include"read_and_write.h"



bool test_command ( string command0, string  command1 )
{
    return command0.compare ( command1 ) == 0;
}

int parse_first_command(string first_command){
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
    else if ( test_command ( first_command, "quit" ) )
        return QUIT;
    else if ( test_command ( first_command, "format" ) )
        return FORMAT;
    else if ( test_command ( first_command, "mkdir" ) )
        return MKDIR;
    else if ( test_command ( first_command, "cd" ) )
        return CHDIR;
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
    printf ( "quit :quit the system\n" );
    printf ( "format :format the system\n" );
    printf("for details can input command+help\n");
}

void ls(deque<string>&  commands) {
    string target_directory;
    if(commands.size ( )>0 )
    target_directory = commands[0];
    if (target_directory.length() == 0){
        _dir();
    } else{
        cout<<"list directory "<<target_directory <<" files\n";
    }
}

void _create ( deque<string>&  commands ) {
    if ( user_id == NOTLOGIN )
    {
        cout << "please login!" << endl;
        return;
    }
    string arg;
    if ( commands.size ( )>0 )
        arg = commands [0];
    if (arg.length ( ) == 0){
        printf("there's something wrong,please check it or input create help to get help\n");
    } else if (test_command(arg, "help")){
        printf("create filename:create the file\n");
    } else{
        const char * filename = arg.c_str();
        int handle = create_file (user_id, filename, DEFAULTMODE);
        close_file ( user_id, handle );
        
    }
}

void _delete( deque<string>&  commands ) {
    if ( user_id == NOTLOGIN )
    {
        cout << "please login!" << endl;
        return;
    }
    string arg;
    if ( commands.size ( )>0 )
        arg = commands [0];
    if ( arg.length ( ) == 0 || (test_command( arg, "all")))
        printf("there's something wrong,please check it or input delete help to get help\n");
    else if (test_command( arg, "help")){
        printf("delete filename:  delete the  file\n");
    } else {
        const char * filename = arg.c_str();
        delete_file ( filename );
    }

}



void read( deque<string>&  commands ) {
    if ( user_id == NOTLOGIN )
    {
        cout << "please login!" << endl;
        return;
    }
    string arg0;
    if ( commands.size ( ) >0 )
    {
        arg0 = commands [0];
    }

    if ( arg0.length ( ) == 0){
        printf("there's something wrong,please check it or input read help to get help\n");
    } else if (test_command( arg0, "help")){
        printf("read filename:read the file\n");
    } else{
        const char* filename = arg0.c_str();
        int handle;
        handle = open_file ( user_id, filename, READ_AB );
        if ( handle == OPEN_FAILED ) { 
            cout << "please check your command\n"; 
            return; }
        if ( handle != OPEN_FAILED ) {
            char *buffer;
            int length = sys_ofile [users [user_id].u_ofile [handle]].f_inode->di_size;
            buffer = ( char* ) malloc ( length + 1);
            read_file ( handle, buffer, length );
            *(buffer + length) = '\0';
            printf ( "%s\n", buffer );
            free ( buffer );
        }
    }
}

void write( deque<string>&  commands ) {
    if ( user_id == NOTLOGIN )
    {
        cout << "please login!" << endl;
        return;
    }
    string arg0;
    string arg1;
    string arg2;
    if ( commands.size ( ) >2 )
    {
        arg0 = commands [0];
        arg1 = commands [1];
        arg2 = commands [2];
    }
    else
    {
        printf ( "there's something wrong,please check it or input write help to get help\n" );
        return;
    }

    if ( test_command ( arg0, "help" ) ) {
        printf("write mode filename:write file\n");
        cout << "mode: w, w+" << endl;
    } else{
        const char* filename = arg0.c_str();
        const char* mode_arg = arg1.c_str();
        int handle;
        if ( strcmp ( mode_arg, "w" ) == 0 )
        {
            handle = open_file ( user_id, filename, WRITE_AB );
        }
        else if ( strcmp ( mode_arg, "w+" ) == 0 )
        {
            handle = open_file ( user_id, filename, ADD_AB );
        }
        else { cout << "please check your command\n"; return; }
        if ( handle != OPEN_FAILED ) {
            char *data;
            int len = arg2.length ( );
            data = ( char * ) malloc ( ( len ) * sizeof ( char ) );
            arg2.copy ( data, len, 0 );
            write_file ( handle, data, len );
            free ( data );
            close_file ( user_id, handle );
        }

    }
}

void login( deque<string>&  commands ) {
    string arg0;
    string arg1;
    if ( commands.size() >1 )
    {
        arg0 = commands [0];
        arg1 = commands [1];
    }
    if (arg0.length ( ) ==0 || (!test_command(arg0, "help") && arg1.length ( ) == 0 ))
        printf("there's something wrong,please check it or input login help to get help\n");
    else if (test_command(arg0, "help")){
        printf("login username password：login the user\n");
    } else {
		
        int login_user_id = atoi(arg0.c_str());
        const char* password = arg1.c_str();
		if (!login_user( login_user_id, password))
		{
			printf_s("login failed, please retry.\n");
		}
		else
		{
			
			printf_s("login ok!\nwelcome %d!\n", users[user_id].u_uid);
		}
    }
}

void _register( deque<string>&  commands ) {
    string arg;
    if ( commands.size ( )>0 )
        arg = commands [0];

    if ( arg.length ( ) != 0 && test_command( arg, "help")){
        printf("register：register the user\n");
    } else{
        int reg_user_id;
        char password [20];
        char password1 [20];
        cout << "please input id" << endl;
        cin >> reg_user_id;
        cout << "please input password" << endl;
        cin >> password;
        cout << "please ensure password" << endl;
        cin >> password1;
        if ( strcmp(password, password1)!=0 )
        {
            cout << "two password not same" << endl;
            return;
        }
        reg_user ( reg_user_id, password );
    }
}


void logout( deque<string>&  commands ) {
    if ( user_id == NOTLOGIN )
    {
        cout << "please login!" << endl;
        return;
    }
    string arg;
    if ( commands.size ( )>0 )
        arg = commands [0];

    if ( arg.length ( ) != 0 && test_command ( arg, "help" ) ) {
        printf ( "register：register the user\n" );
    } else{
        logout_user ( users[user_id].u_uid);
        user_id = NOTLOGIN;

    }
}

void rm ( deque<string>& commands )
{
}


void mv( deque<string>&  commands ) {
    string arg0;
    string arg1;
    if ( commands.size ( ) >1 )
    {
        arg0 = commands [0];
        arg1 = commands [1];
    }
    if ( arg0.length ( ) == 0 || ( !test_command ( arg0, "help" ) && arg1.length ( ) == 0 ) )
        printf("there's something wrong,please check it or input mv help to get help\n");
    else if (test_command(arg0, "help")){
        printf("mv source target：move the file\n");
    } else {
        const char* source = arg0.c_str();
        const char* target = arg1.c_str();
        printf("try to move the file %s\n", source);
    }
}

void cp( deque<string>&  commands ) {
    string arg0;
    string arg1;
    if ( commands.size ( ) >1 )
    {
        arg0 = commands [0];
        arg1 = commands [1];
    }
    if ( arg0.length ( ) == 0 || ( !test_command ( arg0, "help" ) && arg1.length ( ) == 0 ) )
        printf("there's something wrong,please check it or input cp help to get help\n");
    else if (test_command(arg0, "help")){
        printf("cp source target: copy the file\n");
    } else {
        const char* source = arg0.c_str();
        const char* target = arg1.c_str();
        printf("try to copy the file %s\n", source);
    }
}

void _mkdir ( deque<string>&  commands ) {
    if ( user_id == NOTLOGIN )
    {
        cout << "please login!" << endl;
        return;
    }
    string arg;
    if ( commands.size()>0 )
        arg = commands [0];

    if ( arg.length ( ) != 0 && test_command ( arg, "help" ) ) {
        printf ( "mkdir dirname：make a new dir\n" );
    }
    else {
        mkdir( arg.c_str());

    }
}

void _chdir ( deque<string>&  commands ) {
    string arg;
    if ( commands.size ( )>0 )
        arg = commands [0];

    if ( arg.length() != 0 && test_command ( arg, "help" ) ) {
        printf ( "chdir dirname：change to target directory\n" );
    }
    else {
        chdir ( arg.c_str() );
    }
}