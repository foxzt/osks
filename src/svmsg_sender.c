/* svmsg_sender.c */


#include "svmsg_hdr_file.h"

static void             /* SIGCHLD handler */
grimReaper(int sig) {
    int savedErrno;

    savedErrno = errno;                 /* 执行任何系统调用之前保存errno码，因为系统调用可能会对这个全局变量进行修改，在执行完系统调用之后恢复原来的样子 */
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = savedErrno;
}

static void             /* 子进程中执行的任务，回应接收程序的文件请求 */
serveRequest(const struct requestMsg *req) {
    int fd;
    ssize_t numRead;
    struct responseMsg resp;

    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) {  /* 打开接收程序请求的那个文件，如果失败则发送一条标志位为RESP_MT_FAILURE的消息，通知接收程序，文件打开失败（文件不存在或者权限不足） */
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
        msgsnd(req->clientId, &resp, strlen(resp.data) + 1, 0);
        exit(EXIT_FAILURE);
    }

    /* 不断读取文件，将文件中的内容转成自定义数据结构，并将有效的消息标志位设置成RESP_MT_DATA，每次读取RESP_MSG_SIZE（一个约定的大小，为8k） */

    resp.mtype = RESP_MT_DATA;
    while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0)
        if (msgsnd(req->clientId, &resp, numRead, 0) == -1)
            break;

    /* 将自定义数据结构中的mtype设置成RESP_MT_END，标志文件读取结束 */

    resp.mtype = RESP_MT_END;
    msgsnd(req->clientId, &resp, 0, 0);         /* Zero-length mtext */
}

int
main(int argc, char *argv[]) {
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    int serverId;
    struct sigaction sa;

    /* 创建发送程序的消息队列，用于接收接收程序的请求，如果创建时已经存在则先删除已经存在的消息队列在创建新的消息队列 */

    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL |
                                  S_IRUSR | S_IWUSR | S_IWGRP);
    if (serverId == -1) {
        if (errno == EEXIST) {
            serverId = msgget(SERVER_KEY, 0);
            if (serverId != -1) {
                if (msgctl(serverId, IPC_RMID, NULL) == -1) {
                    errExit("msgctl");
                }
            } else {
                errExit("msgget");
            }
            serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL |
                                          S_IRUSR | S_IWUSR | S_IWGRP);
            if (serverId == -1) {
                errExit("msgget");
            }
        } else {
            errExit("msgget");
        }
    }


    /* 为SIGCHLD注册一个信号处理函数 */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    /* 读取请求，在子进程处理请求 */

    for (;;) {
        /* 不断接收请求，将参数msgtyp设置成0，不断接收消息队列中的第一条消息 */
        msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
        if (msgLen == -1) {
            if (errno == EINTR)         /* 被SIGCHLD中断，判断errno并重启msgrcv */
                continue;
            errMsg("msgrcv");
            break;                      /* msgrcv发生错误，退出 */
        }

        pid = fork();
        if (pid == -1) {
            errMsg("fork");
            break;
        }

        if (pid == 0) {
            serveRequest(&req);
            _exit(EXIT_SUCCESS);
        }

        /* 父进程循环接收其他请求 */
    }

    /* 如果msgrcv发生错误或者fork发生错误，删除发送程序的消息队列并退出程序 */

    if (msgctl(serverId, IPC_RMID, NULL) == -1)
        errExit("msgctl");
    exit(EXIT_SUCCESS);
}
