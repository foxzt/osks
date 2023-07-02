/* binary_sems.h
   Header file for binary_sems.c.
*/
#ifndef BINARY_SEMS_H
#define BINARY_SEMS_H

#include "myhdr.h"


extern Boolean bsUseSemUndo;            /* semop() 函数调用时是否使用 SEM_UNDO? */
extern Boolean bsRetryOnEintr;          /* semop() 被信号处理程序中断时是否进行重试? */

int initSemAvailable(int semId, int semNum);

int initSemInUse(int semId, int semNum);

int reserveSem(int semId, int semNum);

int releaseSem(int semId, int semNum);

#endif
