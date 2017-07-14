#pragma once
#include"FILESYS.h"


unsigned int access ( unsigned int user_id, struct inode * inode, unsigned short mode );
int login_user ( unsigned short uid, const char *passwd );
int logout_user ( unsigned short uid );
int reg_user ( unsigned short uid, const char *passwd );