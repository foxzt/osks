/* svshm_hdr_file.h */


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "binary_sems.h"
#include "myhdr.h"

#define SHM_KEY 0x1234          /* 共享内存的key */
#define SEM_KEY 0x5678          /* 信号量集的key */

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)


#define WRITE_SEM 0             /* 对共享内存写操作使用的信号量 */
#define READ_SEM 1              /* 读取共享内存使用的信号量 */

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

struct shmseg {                 /* 使用共享内存通信使用的自定义数据结构 */
    int cnt;                    /* buf中的字节数 */
    char buf[BUF_SIZE];         /* 将发送的数据保存在这里 */
};
