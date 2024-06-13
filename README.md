# EVENT BUTTON

通用按键扫描库

## 简易版事件：按下、松开、长按、长按松开

**状态转移图**

![key](assets/image/key.png)

**使用方式**

```c
#include "key.h"

static KEY_VALUE get_KEY0() {
  if (LL_GPIO_IsInputPinSet(KEY0_GPIO_Port, KEY0_Pin) == 0) {
    return K_PRESS;
  } else {
    return K_RELEASE;
  }
}

static KEY key = {
    .status = KS_RELEASE,
    .count = 0,
    .get = get_KEY0,
};

/* 25ms 周期执行，长按计数阈值 20 （500ms） */
KEY_EVENT k_ev = key_event_check(&key, 20);
```

当无按键事件时 `k_ev` 的值为 `KE_NONE`，其它值：`KE_PRESS`、`KE_RELEASE`、`KE_LONG_PRESS`、`KE_LONG_RELEASE`

## 连击版事件：按下、松开、长按、长按松开、连击、连击结束

**状态转移图**

![combo_key](assets/image/combo_key.png)

**使用方式**

```c
#include "combo_key.h"

static KEY_VALUE getKey(void) {
  if (LL_GPIO_IsInputPinSet(B1_GPIO_Port, B1_Pin) == 1) {
    return K_PRESS;
  } else {
    return K_RELEASE;
  }
}

/* 注册按键，25ms 有效电平过滤，500ms 老化时间，1000ms 长按判定时间 */
#define KEY_ID 0
static KEY keys[] = {
    {
        .id = KEY_ID,
        .get = getKey,
        .custom_data = NULL,
        .valid = 25,
        .ageing = 500,
        .long_press = 1000,
    },
};
key_register(keys, sizeof(keys) / sizeof(keys[0]));

/* 重置按键 */
key_reset(KEY_ID);

/* 修改按键，25ms 有效电平过滤，500ms 老化时间，3000ms 长按判定时间 */
key_modify(KEY_ID, getKey, NULL, 25, 500, 3000);

/* 注销按键 */
key_unregister(KEY_ID);

/* 1ms 周期执行 */
KEY *key = NULL;
COMBO_KEY_FOR_EACH(key) {
    switch (combo_key_event_check(key)) {
        case KE_PRESS: {
            /* 按下 */
        } break;
        case KE_RELEASE: {
            /* 松开 */
        } break;
        case KE_LONG_PRESS: {
            /* 长按 */
        } break;
        case KE_LONG_RELEASE: {
            /* 长按松开 */
        } break;
        case KE_COMBO: {
            /* 连击 */
        } break;
        case KE_COMBO_RELEASE: {
            /* 连击结束 */
        } break;
        default: {
        } break;
    }
}
```

当无按键事件时 `k_ev` 的值为 `KE_NONE`，其它值：`KE_PRESS`、`KE_RELEASE`、`KE_LONG_PRESS`、`KE_LONG_RELEASE`、`KE_COMBO_PRESS`、`KE_COMBO_RELEASE`
