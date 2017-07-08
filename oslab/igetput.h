#pragma once
struct inode * iget(unsigned int dinodeid);
int iput(struct inode * pinode);
