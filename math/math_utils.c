#include "math_utils.h"

/**
 * @brief ʵ����������
 * 
 * @param source ����
 * @param exp �ݴ�
 * @return uint32_t 
 */
uint32_t Xww_pow(uint8_t source, uint8_t exp) {

    uint32_t power = 1;
    uint8_t i = 1;
    for(; i <= exp; i++) {
        power *= source;
    }
    return power;

}

uint32_t Xww_abs(int32_t val) {
    if(val < 0) {
        return (uint32_t) -val;
    }
    return (uint32_t)  val;
}
