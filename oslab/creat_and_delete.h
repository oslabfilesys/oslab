#pragma once
#include <stdio.h>
#include "FILESYS.h"
void create_file(unsigned int user_id, char *filename, unsigned short mode);
void delete_file (char *filename);