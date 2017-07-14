#pragma once
#include <stdio.h>
#include "FILESYS.h"

extern void split ( const string& src, const string& separator, deque<string>& dest );

int create_file ( struct dir& dirctory, unsigned int user_id, const char *filename, unsigned short mode );
void  delete_file ( struct dir &directory, const char * filename );

void copy_file ( const char* sourch_filename, string dest );
void move_file ( const char* sourch_filename, string dest );