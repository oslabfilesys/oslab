#pragma once
#include <stdio.h>
#include "FILESYS.h"
unsigned int read_file(int fd1, char *buf, unsigned int size);
unsigned int write_file(int fd1, char *buf, unsigned int size);