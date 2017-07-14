#pragma once
#include <stdio.h>
#include "FILESYS.h"
unsigned short open_file(int user_id, const char *filename, unsigned short openmode);
void close_file(unsigned int user_id, unsigned short cfd);