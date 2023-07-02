/* binary_sems.c
   Implement a binary semaphore protocol using System V semaphores.
*/

#include <sys/sem.h>
#include "semun.h"
#include "binary_sems.h"
#include "svshm_hdr_file.h"

Boolean bsUseSemUndo = FALSE;
Boolean bsRetryOnEintr = TRUE;

int                     /* 用于初始化一个信号量为可用状态 */
initSemAvailable(int semId, int semNum)
{
    union semun arg;

    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}

int                     /* 用于初始化一个信号量为正在使用的状态 */
initSemInUse(int semId, int semNum)
{
    union semun arg;

    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}

/* 阻塞方式申请信号量，成功时返回0，如果操作被信号处理程序中断，则返回-1，并设置 'errno' 为 EINTR */

int                     /* 用于对信号量进行预留操作，即将信号量的值减少 1 */
reserveSem(int semId, int semNum)
{
    struct sembuf sops;

    sops.sem_num = semNum;
    sops.sem_op = -1;
    sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

    while (semop(semId, &sops, 1) == -1)
        if (errno != EINTR || !bsRetryOnEintr)
            return -1;

    return 0;
}

int                     /* 用于释放信号量，即将信号量的值增加 1 */
releaseSem(int semId, int semNum)
{
    struct sembuf sops;

    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

    return semop(semId, &sops, 1);
}
