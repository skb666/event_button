#ifndef __DATA_STRUCT_H__
#define __DATA_STRUCT_H__

#include <stddef.h>

#define container_of(ptr, type, member) ({             \
    const typeof(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); \
})

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    STRUCT_LINK,
} DATA_STRUCT_TYPE;

typedef struct __LINK_NODE {
    struct __LINK_NODE *next;
} LINK_NODE;

#ifdef __cplusplus
}
#endif

#endif
