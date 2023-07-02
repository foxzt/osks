/* svmsg_receiver.c */


#include "svmsg_hdr_file.h"

static int clientId;

static void
removeQueue(void) {
    if (msgctl(clientId, IPC_RMID, NULL) == -1)
        errExit("msgctl");
}

int
main(int argc, char *argv[]) {
    struct requestMsg req;
    struct responseMsg resp;
    int serverId, numMsgs;
    ssize_t msgLen, totBytes;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    if (strlen(argv[1]) > sizeof(req.pathname) - 1)
        cmdLineErr("pathname too long (max: %ld bytes)\n",
                   (long) sizeof(req.pathname) - 1);

    /* 获取发送程序的消息队列标识符，并创建自己的消息队列用于接收发送程序的回应 */

    serverId = msgget(SERVER_KEY, S_IWUSR);
    if (serverId == -1)
        errExit("msgget - server message queue");

    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId == -1)
        errExit("msgget - client message queue");

    if (atexit(removeQueue) != 0)
        errExit("atexit");

    /* 通过命令行参数的方式指定需要获取的文件路径名，并将这个请求消息发送到发送程序的消息队列 */

    req.mtype = 1;                      /* 这个不重要，随便设置就行 */
    req.clientId = clientId;
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';
    /* 将最后一个设置成'\0' */

    if (msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1)
        errExit("msgsnd");

    /* 阻塞等待发送程序的回应 */

    msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
    if (msgLen == -1)
        errExit("msgrcv");

    if (resp.mtype == RESP_MT_FAILURE) {
        printf("%s\n", resp.data);      /* 返回失败，并打印发送程序回应的失败信息 */
        exit(EXIT_FAILURE);
    }

    /* 如果需要获取的文件成功被发送程序打开，则发送程序会将含有文件内容的消息作为相应发送到接受程序的消息队列 */

    totBytes = msgLen;                  /* 统计第一个消息的消息字节数 */
    for (numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++) {
        msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
        if (msgLen == -1)
            errExit("msgrcv");

        totBytes += msgLen;
    }

    /* 这里简单地统计了一下文件的总字节数，并没有保存文件 */
    printf("Received %ld bytes (%d messages)\n", (long) totBytes, numMsgs);

    exit(EXIT_SUCCESS);
}
