#pragma once
#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#define BLOCKSIZ    512
#define SYSOPENFILE 40
#define DIRNUM      128
#define DIRSIZ      14
#define PWDSIZ      12
#define PWDNUM      32
#define NOFILE      20
#define NADDR       10
#define NHINO       128
#define USERNUM     10
#define DINODESIZ   32
#define DINODEBLK   32 //�������С
#define FILEBLK     512
#define NICFREE     50
#define NICINOD     50
#define DINODESTART 2*BLOCKSIZ
#define DATASTART   (2+DINODEBLK)*BLOCKSIZ
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


struct inode {
	struct inode *i_forw;//ǰ��
	struct inode *i_back;//ָ���һ���ڵ��ָ��
	char i_flag;//��־λ
	unsigned int i_ino;//inode ��
	unsigned int i_count;//inode ���ô���
	unsigned short di_number;//��Ӧ��Ŀ¼��
	unsigned short di_mode;//����ģʽ
	unsigned short di_uid;//�û���
	unsigned short di_gid;//�û����
	unsigned short di_size;//���С
	unsigned int di_addr[NADDR];//�����ݿ�ĵ�ַ�йص�����
};

struct dinode {//������ݽṹ��Ӧinode��Ĳ���
	unsigned short di_number;
	unsigned short di_mode;
	unsigned short di_uid;
	unsigned short di_gid;
	unsigned long di_size;
	unsigned int di_addr[NADDR];
};

struct direct {
	char d_name[DIRSIZ];//Ŀ¼����С
	unsigned int d_ino;//Ŀ¼��Ӧ��inode��
};
//���������ݽṹ
struct filsys {
	unsigned short s_isize;//��Ӧ��inode��С
	unsigned long s_fsize;//��Ӧ�������С
	unsigned int s_nfree;//ָ����п�ָ��
	unsigned short s_pfree;
	unsigned int s_free[NICFREE];//���п�����
	unsigned int s_ninode;
	unsigned short s_pinode;
	unsigned int s_inode[NICINOD];
	unsigned int s_rinode;
	char s_fmod;//����ģʽ
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
	char f_flag;
	unsigned int f_count;
	struct inode *f_inode;
	unsigned long f_off;
};

struct user {
	unsigned short u_default_mode;
	unsigned short u_uid;
	unsigned short u_gid;
	unsigned short u_ofile[NOFILE];
};


extern struct hinode h_inode [NHINO];
extern struct dir directory;
extern struct file sys_ofile[SYSOPENFILE];
extern struct filsys file_system;
extern struct pwd _pwd [PWDNUM];
extern struct user users [USERNUM];
extern        FILE *fd;
extern struct inode *cur_path_inode;
extern int    user_id, file_block;
