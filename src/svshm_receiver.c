/* svshm_receiver.c

    从共享内存中读取信息
*/



#include "svshm_hdr_file.h"

int
main(int argc, char *argv[])
{
    int semid, shmid, xfrs, bytes;
    struct shmseg *shmp;

    /* 获取信号量集和共享内存的标识符 */

    semid = semget(SEM_KEY, 0, 0);
    if (semid == -1)
        errExit("semget");

    shmid  = shmget(SHM_KEY, 0, 0);
    if (shmid == -1)
        errExit("shmget");

    /* 将共享内存附加到程序的虚拟内存地址空间，权限设置成只读 */

    shmp = shmat(shmid, NULL, SHM_RDONLY);
    if (shmp == (void *) -1)
        errExit("shmat");

    /* 将从共享内存读到的自定义数据结构转换成文本信息输出到标准输出 */

    for (xfrs = 0, bytes = 0; ; xfrs++) {
        if (reserveSem(semid, READ_SEM) == -1)          /* 读之前对读信号量进行P操作 */
            errExit("reserveSem");

        if (shmp->cnt == 0)                     /* 已经读取完所有发送程序发送来的数据 */
            break;
        bytes += shmp->cnt;

        if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
            fatal("partial/failed write");

        if (releaseSem(semid, WRITE_SEM) == -1)         /* 对写信号量进行V操作 */
            errExit("releaseSem");
    }

    if (shmdt(shmp) == -1)
        errExit("shmdt");

    /* 再次对写信号量进行V操作，唤醒发送程序，让发送程序进行收尾工作 */

    if (releaseSem(semid, WRITE_SEM) == -1)
        errExit("releaseSem");

    fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
