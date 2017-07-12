#pragma once
#include <string.h>
#include <stdio.h>
#include "FILESYS.h"
unsigned int search_directory_or_file_id_by_name (char *name);
unsigned int put_the_name_to_current_dir (char *name);
void _dir();
void mkdir(char *dirname);
void chdir(char *dirname);