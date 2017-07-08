#include"access.h"
#include<string.h>
#include"igetput.h"

unsigned int access ( unsigned int user_id, struct inode * _inode, unsigned short mode ){
    switch ( mode )
    {
        case READ:
            if ( _inode->di_mode & ODIREAD ) return 1;
            if ( ( _inode->di_mode & ODIREAD ) && ( users [user_id].u_gid == _inode->di_gid ) ) return 1;
            if ( ( _inode->di_mode & UDIREAD ) && ( users [user_id].u_uid == _inode->di_uid ) ) return 1;
            return 0;

        case WRITE:
            if ( _inode->di_mode & ODIWRITE ) return 1;
            if ( ( _inode->di_mode & GDIWRITE ) && ( users [user_id].u_gid == _inode->di_gid ) ) return 1;
            if ( ( _inode->di_mode & UDIWRITE ) &&
                ( users [user_id].u_uid == _inode->di_uid ) ) return 1;
            return 0;

        case EXICUTE:

            if ( _inode->di_mode & ODIEXICUTE ) return 1;
            if ( ( _inode->di_mode & GDIEXICUTE ) &&
                ( users [user_id].u_gid == _inode->di_gid ) ) return 1;
            if ( ( _inode->di_mode & UDIEXICUTE ) &&
                ( users [user_id].u_uid == _inode->di_uid ) ) return 1;
            return 0;
        default:
            return 0;
    }
}
int login ( unsigned short uid, char * passwd )
{
    int i, j;
    for ( i = 0; i<PWDNUM; i++ )
    {
        if ( ( uid == _pwd [i].p_uid ) && !( strcmp ( passwd, _pwd [i].password ) ) )//验证用户名密码
        {
            for ( j = 0; j<USERNUM; j++ )
                if ( users [j].u_uid == 0 ) break;// =0为啥退出
            if ( j == USERNUM )//不需要这句
            {

                printf ( "\nToo much user in the System, waited to login\n" );
                return 0;
            }
            else
            {
                users [j].u_uid = uid;
                users [i].u_gid = _pwd [i].p_gid;
                users [j].u_default_mode = DEFAULTMODE;
            }
            break;
        }
    }

    if ( i == PWDNUM )
    {
        printf ( "\n incorrect password\n" );
        return 0;
    }
    else
        return 1;
}
int logout ( unsigned short uid )
{
    int i, j, sys_no;
    struct inode *inode;
    for ( i = 0; i<USERNUM; i++ )
        if ( uid == users [i].u_uid ) break;
    if ( i == USERNUM )
    {
        printf ( "\nno such a file\n" );
        return NULL;
    }
    for ( j = 0; j<NOFILE; j++ )
    {
        if ( users [i].u_ofile [j] != SYSOPENFILE + 1 )
        {
            /* iput the mode free the sys_ofile and clear the user-ofile */
            sys_no = users [i].u_ofile [j];
            inode = sys_ofile [sys_no].f_inode;
            iput ( inode );
            sys_ofile [sys_no].f_count--;
            users [i].u_ofile [j] = SYSOPENFILE + 1;
        }
    }
    return 1;
}




