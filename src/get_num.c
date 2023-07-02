/* get_num.c
    用于处理数值类型命令行参数的函数
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "get_num.h"

/* 打印诊断消息，其中包含函数名（'fname'），命令行参数值（'arg'），命令行参数的名称（'name'）和诊断错误消息（'msg'） */
static void
gnFail(const char *fname, const char *msg, const char *arg, const char *name) {
    fprintf(stderr, "%s error", fname);
    if (name != NULL)
        fprintf(stderr, " (in %s)", name);
    fprintf(stderr, ": %s\n", msg);
    if (arg != NULL && *arg != '\0')
        fprintf(stderr, "        offending text: %s\n", arg);

    exit(EXIT_FAILURE);
}

/* 将数字型命令行参数（'arg'）转换为长整型，并作为函数结果返回。'flags' 是位掩码，用于控制如何进行转换以及对数字结果进行何种诊断检查；
'fname' 是调用者的名称，'name' 是与命令行参数 'arg' 关联的名称。如果在处理 'arg' 时检测到错误，则使用 'fname' 和 'name' 打印诊断消息 */

static long
getNum(const char *fname, const char *arg, int flags, const char *name) {
    long res;
    char *endptr;
    int base;

    if (arg == NULL || *arg == '\0')
        gnFail(fname, "null or empty string", arg, name);

    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 :
                                       (flags & GN_BASE_16) ? 16 : 10;

    errno = 0;
    res = strtol(arg, &endptr, base);
    if (errno != 0)
        gnFail(fname, "strtol() failed", arg, name);

    if (*endptr != '\0')
        gnFail(fname, "nonnumeric characters", arg, name);

    if ((flags & GN_NONNEG) && res < 0)
        gnFail(fname, "negative value not allowed", arg, name);

    if ((flags & GN_GT_0) && res <= 0)
        gnFail(fname, "value must be > 0", arg, name);

    return res;
}

/* 将数字型命令行参数字符串转换为长整型 */

long
getLong(const char *arg, int flags, const char *name) {
    return getNum("getLong", arg, flags, name);
}

/* 将数字型命令行参数字符串转换为整型 */

int
getInt(const char *arg, int flags, const char *name) {
    long res;

    res = getNum("getInt", arg, flags, name);

    if (res > INT_MAX || res < INT_MIN)
        gnFail("getInt", "integer out of range", arg, name);

    return res;
}
