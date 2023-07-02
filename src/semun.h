/* semun.h */
#ifndef SEMUN_H
#define SEMUN_H

#include <sys/types.h>
#include <sys/sem.h>

union semun {                   /* 用于在调用 semctl() 函数时使用 */
    int val;
    struct semid_ds *buf;
    unsigned short *array;
#if defined(__linux__)
    struct seminfo *__buf;
#endif
};

#endif