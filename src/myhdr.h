#ifndef TLPI_HDR_H
#define TLPI_HDR_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "lib/get_num.h"    /* 用于处理数字参数的函数 */

#include "lib/error_functions.h"  /* 包装错误处理函数 */


#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum {
    FALSE, TRUE
} Boolean;

#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#endif
