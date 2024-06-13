#include "combo_key.h"

#include <stdio.h>

KEY_EVENT key_event_get(KEY *key) {
    if (key == NULL) {
        return KE_ERROR;
    }
    return key->event;
}

int key_combo_press_count(KEY *key) {
    if (key == NULL) {
        return -1;
    }
    return key->press_time;
}

int key_combo_release_count(KEY *key) {
    if (key == NULL) {
        return -1;
    }
    return key->release_time;
}

int8_t key_reset(KEY *key) {
    if (key == NULL) {
        return -1;
    }

    key->status = KS_NONE;
    key->event = KE_NONE;
    key->press_cnt = 0;
    key->release_cnt = 0;
    key->press_time = 0;
    key->release_time = 0;

    return 0;
}

int8_t key_modify(KEY *key, void *custom_data, uint16_t valid, uint16_t ageing, uint16_t long_press) {
    if (key == NULL) {
        return -1;
    }

    if (key->press_cnt >= key->long_press) {
        key->press_cnt = long_press;
    }
    key->valid = valid;
    key->ageing = ageing;
    key->long_press = long_press;
    key->custom_data = custom_data;

    return 0;
}

KEY_EVENT combo_key_event_check(KEY *key) {
    if (key == NULL) {
        return KE_ERROR;
    }

    key->event = KE_NONE;
    switch (key->status) {
        case KS_NONE: {
            if (key->get() == K_PRESS) {
                key->press_cnt = 0;
                key->release_cnt = 0;
                key->press_time = 0;
                key->release_time = 0;
                key->status = KS_SHAKE;
            }
        } break;
        case KS_SHAKE: {
            if (key->get() == K_PRESS) {
                key->press_cnt += 1;
                if (key->press_cnt > key->valid) {
                    key->press_time += 1;
                    key->status = KS_PRESS;
                    if (key->press_time == 1) {
                        key->event = KE_PRESS;
                    } else if (key->press_time > 1) {
                        key->event = KE_COMBO;
                    }
                }
            } else {
                if (key->release_time == 1) {
                    key->event = KE_RELEASE;
                } else if (key->release_time > 1) {
                    key->event = KE_COMBO_RELEASE;
                }
                key->status = KS_NONE;
            }
        } break;
        case KS_PRESS: {
            if (key->get() == K_PRESS) {
                key->press_cnt += 1;
                if (key->press_cnt == key->long_press) {
                    key->event = KE_LONG_PRESS;
                }
            } else {
                key->release_time += 1;
                key->status = KS_RELEASE;
            }
        } break;
        case KS_RELEASE: {
            if (key->press_cnt >= key->long_press) {
                key->event = KE_LONG_RELEASE;
                key->status = KS_NONE;
                break;
            }
            if (key->get() == K_PRESS) {
                key->press_cnt = 0;
                key->release_cnt = 0;
                key->status = KS_SHAKE;
            } else {
                key->release_cnt += 1;
                if (key->release_cnt >= key->ageing) {
                    if (key->release_time == 1) {
                        key->event = KE_RELEASE;
                    } else {
                        key->event = KE_COMBO_RELEASE;
                    }
                    key->status = KS_NONE;
                }
            }
        } break;
    }

    return key->event;
}
