/* svmsg_file.h */


#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stddef.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include "myhdr.h"

#define SERVER_KEY 0x1aaaaaa1           /* 发送程序消息队列的key */

struct requestMsg {                     /* 请求消息自定义数据结构 */
    long mtype;                         /* 没用 */
    int clientId;                      /* 接受程序的消息队列ID */
    char pathname[PATH_MAX];            /* 请求的文件路径名 */
};

/* 使用offsetof()获取requestMsg，消息体的长度 */

#define REQ_MSG_SIZE (offsetof(struct requestMsg, pathname) - \
                      offsetof(struct requestMsg, clientId) + PATH_MAX)

#define RESP_MSG_SIZE 8192

struct responseMsg {                    /* 回应消息的自定义数据结构 */
    long mtype;                         /* 回应消息的类型，是下面定义的三种类型之一 */
    char data[RESP_MSG_SIZE];           /* 请求的文件内容 */
};

/* 发送程序发送给接收程序的消息自定义类型 */

#define RESP_MT_FAILURE 1               /* 文件无法打开 */
#define RESP_MT_DATA    2               /* 包含文件内容的正常消息 */
#define RESP_MT_END     3               /* 文件已经全部发送完毕 */
