#include"access.h"
#include<string.h>
#include"igetput.h"



unsigned int access ( unsigned int user_id, struct inode * _inode, unsigned short mode ){
    switch ( mode )
    {
        case READ_AB:
            if (_inode->di_uid == users[user_id].u_uid  ) return 1;
            else if ( _inode->other_mode & READ_AB )
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        case WRITE_AB:
        case ADD_AB:
            if ( _inode->di_uid == users [user_id].u_uid ) return 1;
            else if ( _inode->other_mode & WriteAble )
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        case DEFAULTMODE:
            if ( _inode->di_uid == users [user_id].u_uid ) return 1;
            else if ( _inode->other_mode & DEFAULTMODE )
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        default:
            return 0;
    }
}
int login_user ( unsigned short uid,const char * passwd )
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
                users [j].u_gid = _pwd [i].p_gid;
                users [j].u_default_mode = DEFAULTMODE;
                user_id = j;
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
int logout_user ( unsigned short uid )
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
    cout << "logout ok!" << endl;
    return 1;
}
int reg_user ( unsigned short uid, const char * passwd )
{
    int i;
    for (  i = 0; i < PWDNUM; i++ )
    {
        if ( _pwd[i].p_uid == uid )
        {
            printf ( "user have already exist!\n" );
            return 0;
        }
        else if ( _pwd [i].p_uid == 0)
        {
            break;
        }
    }
    if ( i == PWDNUM ) {
        printf ( "full, cannot reg,sorry!\n" );
        return 0;
    }

    _pwd [i].p_uid = uid;
    strcpy_s ( _pwd [i].password, passwd );
    cout << "reg success,now you can login!"<<endl;
    return 1;
}

