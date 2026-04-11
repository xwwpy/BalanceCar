#include "system_tick.h"

__IO uint64_t system_tick_ms = 0;

uint64_t inline get_tick(void) {
    return system_tick_ms;
}

uint64_t inline get_tick_us(void) {
    return system_tick_ms * 1000 + ((SysTick->LOAD - SysTick->VAL) / 72);
}


void system_tick_init(void) {
    SysTick_Config(72000); // 每1ms产生一次中断
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    system_tick_ms++;
}
