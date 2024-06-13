#include "event_button.h"

#include <stdio.h>

#define KEY_LINK_FOR_EACH(pos) for ((pos) = key_link_get(NULL); (pos); (pos) = (pos)->next)

static struct {
    KEY_LINK head;
    int num;
} s_key_list = {0};

static LINK_NODE *key_link_get(int *num) {
    if (num != NULL) {
        *num = s_key_list.num;
    }

    return s_key_list.head.node.next;
}

static KEY *key_link_find_by_id(uint16_t id) {
    LINK_NODE *key_node = NULL;

    if (s_key_list.num == 0) {
        return NULL;
    }

    KEY_LINK_FOR_EACH(key_node) {
        if (container_of(key_node, KEY_LINK, node)->key.id == id) {
            break;
        }
    }

    if (key_node == NULL) {
        return NULL;
    }

    return &container_of(key_node, KEY_LINK, node)->key;
}

int8_t key_link_unregister(uint16_t id) {
    LINK_NODE *bk = &s_key_list.head.node;
    LINK_NODE *p = NULL;

    if (s_key_list.num == 0) {
        return -1;
    }

    KEY_LINK_FOR_EACH(p) {
        if (container_of(p, KEY_LINK, node)->key.id == id) {
            bk->next = p->next;
            s_key_list.num -= 1;
            return 0;
        }
        bk = p;
    }

    return -1;
}

int key_link_register(KEY_LINK *keys, int num) {
    KEY *temp = NULL;
    int cnt = 0;

    if ((keys == NULL) || (num <= 0)) {
        return 0;
    }

    for (int i = 0; i < num; ++i) {
        temp = key_link_find_by_id(keys[i].key.id);
        if (temp != NULL) {
            if (temp == &keys[i].key) {
                continue;
            } else {
                key_link_unregister(keys[i].key.id);
            }
        }
        temp = &container_of(s_key_list.head.node.next, KEY_LINK, node)->key;
        s_key_list.head.node.next = &keys[i].node;
        keys[i].node.next = &container_of(temp, KEY_LINK, key)->node;
        cnt += 1;
    }
    s_key_list.num += cnt;

    return cnt;
}

static void key_link_traverse(void (*func)(KEY *)) {
    LINK_NODE *p = NULL;

    if ((s_key_list.num == 0) || (func == NULL)) {
        return;
    }

    KEY_LINK_FOR_EACH(p) {
        func(&container_of(p, KEY_LINK, node)->key);
    }
}

KEY *(*key_find_by_id(DATA_STRUCT_TYPE data_type))(uint16_t) {
    switch (data_type) {
        case STRUCT_LINK: {
            return key_link_find_by_id;
        } break;
        default: {
            return NULL;
        } break;
    }
}

void (*key_traverse(DATA_STRUCT_TYPE data_type))(void (*)(KEY *)) {
    switch (data_type) {
        case STRUCT_LINK: {
            return key_link_traverse;
        } break;
        default: {
            return NULL;
        } break;
    }
}
