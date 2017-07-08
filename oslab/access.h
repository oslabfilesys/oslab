#pragma once
#include"FILESYS.h"

unsigned int access ( unsigned int user_id, struct inode * inode, unsigned short mode );
int login ( unsigned short uid, char *passwd );
int logout ( unsigned short uid );