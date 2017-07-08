#include <stdio.h>
#include "FILESYS.h"
#include"igetput.h"

static struct dinode block_buf[BLOCKSIZ / DINODESIZ];
struct inode * ialloc()
{
	struct inode * temp_inode;
	unsigned int cur_di;
	int i, count, block_end_flag;
	if ( file_system.s_pinode == NICINOD)
	{
		i = 0;
		count = 0;
		block_end_flag = 1;
        file_system.s_pinode = NICINOD - 1;
		cur_di = file_system.s_rinode;
		while ((count <NICINOD) || (count <= file_system.s_ninode))
		{
			if (block_end_flag)
			{
				fseek(fd, DINODESTART + cur_di*DINODESIZ, SEEK_SET);
				fread(block_buf, 1, BLOCKSIZ, fd);
				block_end_flag = 0;
				i = 0;
			}
			while (block_buf[i].di_mode == DIEMPTY)
			{
				cur_di++;
				i++;
			}
			if (i == NICINOD)
				block_end_flag = 1;
			else
			{
                file_system.s_inode[file_system.s_pinode--] = cur_di;
				count++;
			}
		}
        file_system.s_rinode = cur_di;
	}
	temp_inode = iget(file_system.s_inode[file_system.s_pinode]);
	fseek(fd, DINODESTART + file_system.s_inode[file_system.s_pinode] * DINODESIZ, SEEK_SET);
	fwrite(&temp_inode->di_number, 1, sizeof(struct dinode), fd);
    file_system.s_pinode++;
    file_system.s_ninode--;
    file_system.s_fmod = SUPDATE;
	return temp_inode;
}
void ifree(unsigned dinodeid)	 /* ifree */
{
    file_system.s_ninode++;
	if ( file_system.s_pinode != NICINOD)    /* notfull */
	{
        file_system.s_inode[file_system.s_pinode] = dinodeid;
        file_system.s_pinode++;
	}
	else /* full */
	{
		if (dinodeid <file_system.s_rinode)
		{
            file_system.s_inode[NICINOD] = dinodeid;
            file_system.s_rinode = dinodeid;
		}
	}
}