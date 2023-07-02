/*  svshm_xfr_writer.c
        往共享内存中写入信息
        $ svshm_sender < infile &
        $ svshm_receiver > out_file
*/
#include "semun.h"              /* 定义 semun 联合体 */
#include "svshm_hdr_file.h"

int
main(int argc, char *argv[])
{
    int semid, shmid, bytes, xfrs;
    struct shmseg *shmp;
    union semun dummy;

    /* 创建一个包含两个信号量的信号量集，做好初始化，以保证发送程序先访问共享内存 */

    semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
    if (semid == -1)
        errExit("semget");

    if (initSemAvailable(semid, WRITE_SEM) == -1)
        errExit("initSemAvailable");
    if (initSemInUse(semid, READ_SEM) == -1)
        errExit("initSemInUse");

    /* 创建共享内存，将其附加到程序虚拟内存地址空间 */

    shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
    if (shmid == -1)
        errExit("shmget");

    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *) -1)
        errExit("shmat");

    /* 从stdin读取的数据转换成自定义数据块 */

    for (xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt) {
        if (reserveSem(semid, WRITE_SEM) == -1)         /* 进行写操作前对写信号量做P操作 */
            errExit("reserveSem");

        shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
        if (shmp->cnt == -1)
            errExit("read");

        if (releaseSem(semid, READ_SEM) == -1)          /* 都信号量V操作 */
            errExit("releaseSem");

        /* 当读到EOF代表读入结束，将cnt设置成0 */

        if (shmp->cnt == 0)
            break;
    }

    /* 对读信号量做P操作，确保接收程序最后一次访问共享内存成功前不删除共享内存 */

    if (reserveSem(semid, WRITE_SEM) == -1)
        errExit("reserveSem");

    if (semctl(semid, 0, IPC_RMID, dummy) == -1)
        errExit("semctl");
    if (shmdt(shmp) == -1)
        errExit("shmdt");
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        errExit("shmctl");

    fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
