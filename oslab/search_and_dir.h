#pragma once
#include <string.h>
#include <stdio.h>
#include "FILESYS.h"
unsigned int search_directory_or_file_id_by_name ( struct dir directory, const char *name );
unsigned int put_the_name_to_current_dir ( struct dir &directory, const char *name );
void _dir();
void mkdir( const char *dirname);
void chdir( const char *dirname);