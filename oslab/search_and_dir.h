#pragma once
#include <string.h>
#include <stdio.h>
#include "FILESYS.h"
unsigned int namei(char *name);
unsigned int iname(char *name);
void _dir();
void mkdir(char *dirname);
void chdir(char *dirname);