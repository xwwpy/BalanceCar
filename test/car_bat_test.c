#include "car_bat_test.h"
static uint32_t last_show_info_time = 0;

static void car_bat_debug_print(void) {
    printf("v: %.2f\r\n", car_get_current_v());
}

void car_bat_debug_print_proc(void) {
    uint32_t current_time = get_current_ms();
    if (current_time - last_show_info_time > SHOW_INFO_INTERVAL) {
        car_bat_debug_print();
        last_show_info_time = current_time;
    } else if (current_time < last_show_info_time) { // 处理计时器溢出的情况
        last_show_info_time = current_time;
    }
}
