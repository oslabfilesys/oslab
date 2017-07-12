#pragma once
#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#define BLOCKSIZ    512
#define SYSOPENFILE 160
#define DIRNUM      128
#define DIRSIZ      14
#define PWDSIZ      12
#define PWDNUM      32
#define NOFILE      20
#define NADDR       10
#define NumOfHashIndexNode       128
#define USERNUM     10
#define DiskIndexNodeSize   72 //索引节点块大小被改变！！！！！数据块开始地址需要重新计算
#define DiskIndexNodeBlockNumber   32 //索引节点占的block个数
#define FileMaxBlockNumber     512 //系统允许最多块数	
#define NICFREE     50
#define NICINOD     50
#define DiskIndexNodeStart 2*BLOCKSIZ
#define DATASTART   2*BLOCKSIZ+DiskIndexNodeBlockNumber*DiskIndexNodeSize
#define DIEMPTY     00000
#define DIFILE      01000
#define DIDIR       02000
#define UDIREAD     00001
#define UDIWRITE    00002
#define UDIEXICUTE  00004
#define GDIREAD     00010
#define GDIWRITE    00020
#define GDIEXICUTE  00040
#define ODIREAD     00100
#define ODIWRITE    00200
#define ODIEXICUTE  00400
#define READ        1
#define WRITE       2
#define EXICUTE     3
#define DEFAULTMODE 1
#define IUPDATE     00002
#define SUPDATE     00001
#define FREAD       00001
#define FWRITE      00002
#define FAPPEND     00004
#define DISKFULL    65535
#define SEEK_SET    0
#define UserOpenFileOverflow 21
#define NotHaveInode 6666


struct inode {
	struct inode *i_forw;//前驱
	struct inode *i_back;//指向后一个节点的指针
	char i_flag;//标志位
	unsigned int i_ino;//inode 号
	unsigned int i_count;//inode 引用次数
	unsigned int di_addr[NADDR];//文件的物理块号
	unsigned short di_number;//关联的文件数
	unsigned short di_mode;//访问模式
	unsigned short di_uid;//用户号
	unsigned short di_gid;//用户组号
	unsigned short di_size;//文件大小
};

struct dinode {//这个数据结构对应inode里的参数
	unsigned short di_number;
	unsigned short di_mode;
	unsigned short di_uid;
	unsigned short di_gid;
	unsigned long di_size;
	unsigned int di_addr[NADDR];
};

struct direct {
	char d_name[DIRSIZ];//目录名大小
	unsigned int d_ino;//目录对应的inode号
};
//超级快数据结构
struct filsys {
	unsigned short s_isize;//inode占用的block块数
	unsigned long s_fsize;//data block总块数

	unsigned int s_nfree;//空闲block数
	unsigned short s_pfree;//空闲块指针
	unsigned int s_free[NICFREE];//空闲块数组

	unsigned int surplus_nunmber_of_inode;//剩余inode节点个数
	unsigned short surplus_point_to_inode;//空闲inode指针
	unsigned int s_inode[NICINOD];//空闲inode数组
	unsigned int s_rinode;//下一组inode的开始地点
	char s_fmod;//访问模式
};

struct pwd {
	unsigned short p_uid;
	unsigned short p_gid;
	char password[PWDSIZ];
};

struct dir {
	struct direct direct[DIRNUM];
	int size;
};

struct hinode {
	struct inode *i_forw;
};

struct file {
	char f_flag;//文件操作标志
	unsigned int f_count;//引用计数
	struct inode *f_inode;//内存索引节点
	unsigned long f_off;//读写指针
};

struct user {
	unsigned short u_default_mode;
	unsigned short u_uid;
	unsigned short u_gid;
	unsigned short u_ofile[NOFILE];
};


extern struct hinode hash_index_node [NumOfHashIndexNode];
extern struct dir directory;
extern struct file sys_ofile[SYSOPENFILE];
extern struct filsys file_system;
extern struct pwd _pwd [PWDNUM];
extern struct user users [USERNUM];
extern        FILE *fd;
extern struct inode *cur_path_inode;
extern int    user_id;
