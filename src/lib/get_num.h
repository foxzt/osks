/* get_num.h
   get_num.c的头文件
*/
#ifndef GET_NUM_H
#define GET_NUM_H

#define GN_NONNEG       01      /* 值必须大于等于0 */
#define GN_GT_0         02      /* 值必须大于0 */

#define GN_ANY_BASE   0100      /* 可以使用任何基数 */
#define GN_BASE_8     0200      /* 值以八进制形式表示 */
#define GN_BASE_16    0400      /* 值以十六进制形式表示 */

long getLong(const char *arg, int flags, const char *name);

int getInt(const char *arg, int flags, const char *name);

#endif
