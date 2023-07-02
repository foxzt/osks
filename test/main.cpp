#include <cstdio>
#include <cstddef>
#include <climits>

// 定义一个结构体
struct MyStruct {
    int a;
    char b;
    double c;
};

struct requestMsg {                     /* Requests (client to server) */
    long mtype;                         /* Unused */
    int clientId;                      /* ID of client's message queue */
    char pathname[PATH_MAX];            /* File to be returned */
};

#define REQ_MSG_SIZE (offsetof(struct requestMsg, pathname) - \
                      offsetof(struct requestMsg, clientId) + PATH_MAX)

int main() {
    // 使用 offsetof 宏获取成员的偏移量
    size_t offset = offsetof(struct MyStruct, c);

    printf("成员 c 的偏移量：%zu\n", REQ_MSG_SIZE);

    return 0;
}
