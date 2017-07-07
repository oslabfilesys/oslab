#pragma once
struct inode * iget(unsigned int dinodeid);
struct inode * iput(struct inode * pinode);
