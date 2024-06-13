#ifndef __EVENT_BUTTON_H__
#define __EVENT_BUTTON_H__

#include <stdint.h>

#include "combo_key.h"
#include "data_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    KEY key;
    LINK_NODE node;
} KEY_LINK;

int8_t key_link_unregister(uint16_t id);
int key_link_register(KEY_LINK *keys, int num);

KEY *(*key_find_by_id(DATA_STRUCT_TYPE data_type))(uint16_t);
void (*key_traverse(DATA_STRUCT_TYPE data_type))(void (*)(KEY *));

#ifdef __cplusplus
}
#endif

#endif
