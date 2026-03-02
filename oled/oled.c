#include "oled.h"
// -32????????????????????????????? ???????????????

void set_scl(u8 val) {
    if (val) {
        GPIO_SetBits(GPIOB, GPIO_Pin_8);
    } else {
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
    }
}

void set_sda(u8 val) {
    if (val) {
        GPIO_SetBits(GPIOB, GPIO_Pin_9);
    } else {
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
    }
}


u8 get_sda() {
    return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
}

const u8 OLED_Font[][16] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  0

        0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x00, //! 1

        0x00, 0x10, 0x0C, 0x06, 0x10, 0x0C, 0x06, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //" 2

        0x40, 0xC0, 0x78, 0x40, 0xC0, 0x78, 0x40, 0x00,
        0x04, 0x3F, 0x04, 0x04, 0x3F, 0x04, 0x04, 0x00, // # 3

        0x00, 0x70, 0x88, 0xFC, 0x08, 0x30, 0x00, 0x00,
        0x00, 0x18, 0x20, 0xFF, 0x21, 0x1E, 0x00, 0x00, //$ 4

        0xF0, 0x08, 0xF0, 0x00, 0xE0, 0x18, 0x00, 0x00,
        0x00, 0x21, 0x1C, 0x03, 0x1E, 0x21, 0x1E, 0x00, //% 5

        0x00, 0xF0, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00,
        0x1E, 0x21, 0x23, 0x24, 0x19, 0x27, 0x21, 0x10, //& 6

        0x10, 0x16, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //' 7

        0x00, 0x00, 0x00, 0xE0, 0x18, 0x04, 0x02, 0x00,
        0x00, 0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x00, //( 8

        0x00, 0x02, 0x04, 0x18, 0xE0, 0x00, 0x00, 0x00,
        0x00, 0x40, 0x20, 0x18, 0x07, 0x00, 0x00, 0x00, //) 9

        0x40, 0x40, 0x80, 0xF0, 0x80, 0x40, 0x40, 0x00,
        0x02, 0x02, 0x01, 0x0F, 0x01, 0x02, 0x02, 0x00, //* 10

        0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x01, 0x00, //+ 11

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x80, 0xB0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, //, 12

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, //- 13

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, //. 14

        0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 0x04,
        0x00, 0x60, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, /// 15

        0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00,
        0x00, 0x0F, 0x10, 0x20, 0x20, 0x10, 0x0F, 0x00, // 0 16

        0x00, 0x10, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // 1 17

        0x00, 0x70, 0x08, 0x08, 0x08, 0x88, 0x70, 0x00,
        0x00, 0x30, 0x28, 0x24, 0x22, 0x21, 0x30, 0x00, // 2 18

        0x00, 0x30, 0x08, 0x88, 0x88, 0x48, 0x30, 0x00,
        0x00, 0x18, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00, // 3 19

        0x00, 0x00, 0xC0, 0x20, 0x10, 0xF8, 0x00, 0x00,
        0x00, 0x07, 0x04, 0x24, 0x24, 0x3F, 0x24, 0x00, // 4 20

        0x00, 0xF8, 0x08, 0x88, 0x88, 0x08, 0x08, 0x00,
        0x00, 0x19, 0x21, 0x20, 0x20, 0x11, 0x0E, 0x00, // 5 21

        0x00, 0xE0, 0x10, 0x88, 0x88, 0x18, 0x00, 0x00,
        0x00, 0x0F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00, // 6 22

        0x00, 0x38, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00,
        0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, // 7 23

        0x00, 0x70, 0x88, 0x08, 0x08, 0x88, 0x70, 0x00,
        0x00, 0x1C, 0x22, 0x21, 0x21, 0x22, 0x1C, 0x00, // 8 24

        0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00,
        0x00, 0x00, 0x31, 0x22, 0x22, 0x11, 0x0F, 0x00, // 9 25

        0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, //: 26

        0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, //; 27

        0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00,
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, //< 28

        0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00,
        0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, //= 29

        0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00,
        0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, //> 30

        0x00, 0x70, 0x48, 0x08, 0x08, 0x08, 0xF0, 0x00,
        0x00, 0x00, 0x00, 0x30, 0x36, 0x01, 0x00, 0x00, //? 31

        0xC0, 0x30, 0xC8, 0x28, 0xE8, 0x10, 0xE0, 0x00,
        0x07, 0x18, 0x27, 0x24, 0x23, 0x14, 0x0B, 0x00, //@ 32

        0x00, 0x00, 0xC0, 0x38, 0xE0, 0x00, 0x00, 0x00,
        0x20, 0x3C, 0x23, 0x02, 0x02, 0x27, 0x38, 0x20, // A 33

        0x08, 0xF8, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00,
        0x20, 0x3F, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00, // B 34

        0xC0, 0x30, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00,
        0x07, 0x18, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, // C 35

        0x08, 0xF8, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00,
        0x20, 0x3F, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // D 36

        0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00,
        0x20, 0x3F, 0x20, 0x20, 0x23, 0x20, 0x18, 0x00, // E 37

        0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00,
        0x20, 0x3F, 0x20, 0x00, 0x03, 0x00, 0x00, 0x00, // F 38

        0xC0, 0x30, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00,
        0x07, 0x18, 0x20, 0x20, 0x22, 0x1E, 0x02, 0x00, // G 39

        0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08,
        0x20, 0x3F, 0x21, 0x01, 0x01, 0x21, 0x3F, 0x20, // H 40

        0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0x00,
        0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // I 41

        0x00, 0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00,
        0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, 0x00, // J 42

        0x08, 0xF8, 0x88, 0xC0, 0x28, 0x18, 0x08, 0x00,
        0x20, 0x3F, 0x20, 0x01, 0x26, 0x38, 0x20, 0x00, // K 43

        0x08, 0xF8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x20, 0x3F, 0x20, 0x20, 0x20, 0x20, 0x30, 0x00, // L 44

        0x08, 0xF8, 0xF8, 0x00, 0xF8, 0xF8, 0x08, 0x00,
        0x20, 0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x20, 0x00, // M 45

        0x08, 0xF8, 0x30, 0xC0, 0x00, 0x08, 0xF8, 0x08,
        0x20, 0x3F, 0x20, 0x00, 0x07, 0x18, 0x3F, 0x00, // N 46

        0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00,
        0x0F, 0x10, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // O 47

        0x08, 0xF8, 0x08, 0x08, 0x08, 0x08, 0xF0, 0x00,
        0x20, 0x3F, 0x21, 0x01, 0x01, 0x01, 0x00, 0x00, // P 48

        0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00,
        0x0F, 0x18, 0x24, 0x24, 0x38, 0x50, 0x4F, 0x00, // Q 49

        0x08, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00,
        0x20, 0x3F, 0x20, 0x00, 0x03, 0x0C, 0x30, 0x20, // R 50

        0x00, 0x70, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00,
        0x00, 0x38, 0x20, 0x21, 0x21, 0x22, 0x1C, 0x00, // S 51

        0x18, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x18, 0x00,
        0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, // T 52

        0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08,
        0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00, // U 53

        0x08, 0x78, 0x88, 0x00, 0x00, 0xC8, 0x38, 0x08,
        0x00, 0x00, 0x07, 0x38, 0x0E, 0x01, 0x00, 0x00, // V 54

        0xF8, 0x08, 0x00, 0xF8, 0x00, 0x08, 0xF8, 0x00,
        0x03, 0x3C, 0x07, 0x00, 0x07, 0x3C, 0x03, 0x00, // W 55

        0x08, 0x18, 0x68, 0x80, 0x80, 0x68, 0x18, 0x08,
        0x20, 0x30, 0x2C, 0x03, 0x03, 0x2C, 0x30, 0x20, // X 56

        0x08, 0x38, 0xC8, 0x00, 0xC8, 0x38, 0x08, 0x00,
        0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, // Y 57

        0x10, 0x08, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00,
        0x20, 0x38, 0x26, 0x21, 0x20, 0x20, 0x18, 0x00, // Z 58

        0x00, 0x00, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x00,
        0x00, 0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x00, //[ 59

        0x00, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0x06, 0x38, 0xC0, 0x00, //\ 60

        0x00, 0x02, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00,
        0x00, 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 0x00, //] 61

        0x00, 0x00, 0x04, 0x02, 0x02, 0x02, 0x04, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //^ 62

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, //_ 63

        0x00, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //` 64

        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00,
        0x00, 0x19, 0x24, 0x22, 0x22, 0x22, 0x3F, 0x20, // a 65

        0x08, 0xF8, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00,
        0x00, 0x3F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00, // b 66

        0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00,
        0x00, 0x0E, 0x11, 0x20, 0x20, 0x20, 0x11, 0x00, // c 67

        0x00, 0x00, 0x00, 0x80, 0x80, 0x88, 0xF8, 0x00,
        0x00, 0x0E, 0x11, 0x20, 0x20, 0x10, 0x3F, 0x20, // d 68

        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00,
        0x00, 0x1F, 0x22, 0x22, 0x22, 0x22, 0x13, 0x00, // e 69

        0x00, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x18,
        0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // f 70

        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
        0x00, 0x6B, 0x94, 0x94, 0x94, 0x93, 0x60, 0x00, // g 71

        0x08, 0xF8, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00,
        0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20, // h 72

        0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // i 73

        0x00, 0x00, 0x00, 0x80, 0x98, 0x98, 0x00, 0x00,
        0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, // j 74

        0x08, 0xF8, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00,
        0x20, 0x3F, 0x24, 0x02, 0x2D, 0x30, 0x20, 0x00, // k 75

        0x00, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // l 76

        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
        0x20, 0x3F, 0x20, 0x00, 0x3F, 0x20, 0x00, 0x3F, // m 77

        0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00,
        0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20, // n 78

        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00,
        0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00, // o 79

        0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00,
        0x80, 0xFF, 0xA1, 0x20, 0x20, 0x11, 0x0E, 0x00, // p 80

        0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00,
        0x00, 0x0E, 0x11, 0x20, 0x20, 0xA0, 0xFF, 0x80, // q 81

        0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00,
        0x20, 0x20, 0x3F, 0x21, 0x20, 0x00, 0x01, 0x00, // r 82

        0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
        0x00, 0x33, 0x24, 0x24, 0x24, 0x24, 0x19, 0x00, // s 83

        0x00, 0x80, 0x80, 0xE0, 0x80, 0x80, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x00, 0x00, // t 84

        0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00,
        0x00, 0x1F, 0x20, 0x20, 0x20, 0x10, 0x3F, 0x20, // u 85

        0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80,
        0x00, 0x01, 0x0E, 0x30, 0x08, 0x06, 0x01, 0x00, // v 86

        0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x80,
        0x0F, 0x30, 0x0C, 0x03, 0x0C, 0x30, 0x0F, 0x00, // w 87

        0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00,
        0x00, 0x20, 0x31, 0x2E, 0x0E, 0x31, 0x20, 0x00, // x 88

        0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80,
        0x80, 0x81, 0x8E, 0x70, 0x18, 0x06, 0x01, 0x00, // y 89

        0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
        0x00, 0x21, 0x30, 0x2C, 0x22, 0x21, 0x30, 0x00, // z 90

        0x00, 0x00, 0x00, 0x00, 0x80, 0x7C, 0x02, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x40, 0x40, //{ 91

        0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, //| 92

        0x00, 0x02, 0x02, 0x7C, 0x80, 0x00, 0x00, 0x00,
        0x00, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00, 0x00, //} 93

        0x00, 0x06, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //~ 94
};
void OLED_IIC_send_initial_info() {
    set_scl(1);
    set_sda(1);
    set_sda(0);
    set_scl(0);
}

void OLED_IIC_send_byte(u8 _data) {
    u8 i;
    for (i = 8; i > 0; i--) {
        if ((_data & 0x80) == 0x00) {
            set_sda(0);
        } else {
            set_sda(1);
        }
        _data <<= 1;
        set_scl(1);
        set_scl(0);
    }
}

u8 OLED_IIC_receive_ack() {
    u8 ack;
    set_sda(1);
    set_scl(1);
    ack = get_sda();
    set_scl(0);
    return ack;
}

u8 OLED_IIC_receive_byte() {
    u8 i;
    u8 _data = 0;
    u8 cur_data;
    set_sda(1);
    for (i = 8; i > 0; i--) {
        set_scl(1);
        cur_data = get_sda();
        _data |= (cur_data << (i - 1));
        set_scl(0);
    }
    return _data;
}

void OLED_IIC_send_ack(u8 ack) {
    set_sda(ack);
    set_scl(1);
    set_scl(0);
}

void OLED_IIC_send_stop() {
    set_sda(0);
    set_scl(1);
    set_sda(1);
}

void oled_write_data(u8* bytes, u8 length) {
    OLED_IIC_send_initial_info();
    OLED_IIC_send_byte(OLED_ADDRESS);
    OLED_IIC_receive_ack();
    OLED_IIC_send_byte(OLED_DATA_CONTINUE_MODE);
    OLED_IIC_receive_ack();
    while (length--) {
        OLED_IIC_send_byte(*bytes);
        OLED_IIC_receive_ack();
        bytes++;
    }
    OLED_IIC_send_stop();
}

void oled_write_command(u8* command, u8 length) {
    OLED_IIC_send_initial_info();
    OLED_IIC_send_byte(OLED_ADDRESS);
    OLED_IIC_receive_ack();
    OLED_IIC_send_byte(OLED_COMMAND_CONTINUE_MODE);
    OLED_IIC_receive_ack();
    while (length--) {
        OLED_IIC_send_byte(*command);
        OLED_IIC_receive_ack();
        command++;
    }
    OLED_IIC_send_stop();
}

/** 
 * @brief ??? oled stable
 */
void clear_oled() {
    fill_oled(0);
}

/**
 * @brief ????????? oled stable
 * 
 * @param val ???? 0: ???0 1: ???1
 */
void fill_oled(u8 val) {
    if (val) {
        fill_oled_with_char(0xff, OLED_MAX_PAGE, OLED_MAX_COLUMN, 0, 0, OLED_HORIZONTAL_ADDRESSING_MODE);
    } else {
        fill_oled_with_char(0x00, OLED_MAX_PAGE, OLED_MAX_COLUMN, 0, 0, OLED_HORIZONTAL_ADDRESSING_MODE);
    }
}

/**
 * @brief ???????? stable
 * 
 * @param mode ?????
 */
void oled_set_addressing_mode(u8 mode) {
    u8 cmds[2] = {OLED_SET_ADDRESSING_MODE, 0};
    cmds[1] = mode;
    oled_write_command(cmds, 2);
}

/**
 * @brief ?????????????????????????????????? stable
 * ????????????? ??????????????????????????????????????????????
 * @param start ???????? ????7??
 * @param end   ????????? ????7??
 */
void oled_set_column_address(u8 start, u8 end)
{
    u8 cmds[3] = {OLED_SET_COLUMN_ADDRESS, 0, 0};
    cmds[1] = start & 0x7F;
    cmds[2] = end & 0x7F;
    oled_write_command(cmds, 3);
}

/**
 * @brief ????????
 * 
 */
void reset_column_address() {
    oled_set_column_address(0, OLED_MAX_COLUMN - 1); // ????????
}

/**
 * @brief ??????????????????????????????? stable
 * 
 * @param start ??????? ????3??
 * @param end   ???????? ????3??
 */
void oled_set_page_address(u8 start, u8 end) {
    u8 cmds[3] = {OLED_SET_PAGE_ADDRESS, 0, 0};
    cmds[1] = start & 0x07;
    cmds[2] = end & 0x07;
    oled_write_command(cmds, 3);
}

void reset_page_address() {
    oled_set_page_address(0, 7);
}
/**
 * @brief ????????????????????????
 * 
 * @param col ????? ????5??-7??
 */
void oled_set_column_start_address_higher_nibble(u8 col) {
    u8 cmd = 0x10 | ((col & 0x70) >> 4);
    oled_write_command(&cmd, 1);
}
/**
 * @brief ???????????????? stable
 * 
 * @param page ???? ????3?? 
 */
void oled_set_start_page_address(u8 page) {
    u8 cmd = 0xB0 | (page & 0x07);
    oled_write_command(&cmd, 1);
}
/**
 * @brief ????????????page?????????? stable
 * 
 * @param page ???? ????3??
 * @param col ????? ????7??
 */
void oled_set_page_address_start_column_and_page(u8 page, u8 col) {
    u8 cmds[3];
    cmds[0] = 0xB0 | (page & 0x07); // ????????
    cmds[1] = 0x00 | (col & 0x0F); // ?????4???????
    cmds[2] = 0x10 | ((col & 0x70) >> 4); // ?????3???????
    oled_write_command(cmds, 3);
}

/**
 * @brief ????????????????? stable
 * 
 * @param col ????? ????7??
 */
void oled_set_page_address_start_column(u8 col) {
    u8 cmds[2];
    cmds[0] = 0x00 | (col & 0x0F); // ?????4???????
    cmds[1] = 0x10 | ((col & 0x70) >> 4); // ?????3???????
    oled_write_command(cmds, 2);
}

/**
 * @brief ???????????????????????? stable
 * 
 * @param col ????? ????4??
 */
void oled_set_column_start_address_lower_nibble(u8 col) {
    u8 cmd = 0x00 | (col & 0x0F);
    oled_write_command(&cmd, 1);
}

/**
 * @brief ?????? remap stable
 * @param remap 0: ???? 1: ????
 */
void oled_set_column_remap(u8 remap) {
    u8 cmd;
    if (remap == 0) {
        cmd = OLED_SET_COLUMN_REMAP_DEFAULT;
    } else {
        cmd = OLED_SET_COLUMN_REMAP_REMAP;
    }
    oled_write_command(&cmd, 1);
}

/**
 * @brief ???????????? stable
 * @param remap 0: ???? 1: ????
 */
void oled_set_row_scan_direction(u8 remap) {
    u8 cmd;
    if (remap == 0) {
        cmd = OLED_SET_ROW_SCAN_DEFAULT_DIRECTION;
    } else {
        cmd = OLED_SET_ROW_REVERSE_SCAN_DIRECTION;
    }
    oled_write_command(&cmd, 1);
}

/**
 * @brief ????????? stable
 * @param mode 0: ??????? 1: ??????
 */
void oled_set_display_mod(u8 mode) {
    u8 cmd;
    if (mode == 0) {
        cmd = OLED_SET_DISPLAY_MODE_DEFAULT;
    } else {
        cmd = OLED_SET_DISPLAY_MODE_REVERSE;
    }
    oled_write_command(&cmd, 1);
}

/**
 * @brief ??????? stable
 * 
 */
void oled_display_on() {
    u8 cmd = OLED_DISPLAY_ON;
    oled_write_command(&cmd, 1);
}
/**
 * @brief ???????? stable
 * 
 */
void oled_display_off() {
    u8 cmd = OLED_DISPLAY_OFF;
    oled_write_command(&cmd, 1);
}

/**
 * @brief ???????? stable
 * 
 */
void oled_enable_charge_pump() {
    u8 cmds[2] = {OLED_CHARGE_PUMP_ENABLE};
    oled_write_command(cmds, 2);
}


/**
 * @brief ???????????? ????????????????????? stable
 * @param bytes ???????
 * @param r_len ????
 * @param c_len ????
 * @param start_p_x ????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param start_p_y ????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 */
void oled_write_datas(u8* bytes, u8 r_len, u8 c_len, u8 start_p_x, u8 start_p_y) {
    // ???????????????
    oled_set_column_address(start_p_x, start_p_x + c_len - 1);
    oled_set_page_address(start_p_y, start_p_y + r_len - 1);
    oled_write_data(bytes, r_len * c_len);
}

/**
 * @brief ??????????????????????? ???????????????????? stable
 * @param c ???
 * @param p_x ?????????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param p_y ?????????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 */
void oled_default_show_char(u8 chr, u8 p_x, u8 p_y) {
    u8 default_char_page_size = OLED_DEFAULT_ROW_SIZE / 8;
    // ????????????????
    oled_set_addressing_mode(OLED_HORIZONTAL_ADDRESSING_MODE);
    // ???????????????
    oled_set_column_address(p_x * OLED_DEFAULT_COLUMN_SIZE, p_x * OLED_DEFAULT_COLUMN_SIZE + OLED_DEFAULT_COLUMN_SIZE - 1);
    oled_set_page_address(p_y * default_char_page_size, p_y * default_char_page_size + default_char_page_size - 1);
    oled_write_data(get_char_data_pointer(chr), OLED_DEFAULT_COLUMN_SIZE * default_char_page_size);
    // 每次切换寻址方式都要恢复
    reset_column_address();
    reset_page_address();
}


/**
 * @brief ????????????????? ????????????????????? stable
 * 
 * @param x ?????????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ?????????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param str ????????
 * @param len ????????? ????????????
 */
void oled_default_show_string_with_cut(u8 x, u8 y, u8* str, u8 len) {
    u8 max_row = OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8);
    u8 max_col = OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE;
    if (y >= max_row) {
        return;
    }
    if (x >= max_col) {
        return;
    }
    // ???????????page ?? column
    if (x + len > max_col) {
        len = max_col - x; // ?????????????
    }
    while (len) {
        oled_default_show_char(*str++, x++, y);
        len--;
    }
    
}

/**
 * @brief ????????????????? ?????????????????? stable
 * ???????????????????? ?????
 * @param x ?????????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ?????????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param str ????????
 * @param len ????????? ????????????
 */
void oled_default_show_string_with_no_cut(u8 x, u8 y, u8* str, u8 len) {
    u8 max_row = OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8);
    u8 max_col = OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE;
    u8 original_len = len;
    u8 i = 0;
    if (y >= max_row) {
        return;
    }
    if (x >= max_col) {
        return;
    }
    // ???????????page ?? column
    if (x + len > max_col) {
        len = max_col - x; // ?????????????
    }
    
    for (; i < len; i++) {
        oled_default_show_char(*str, x++, y);
        str += 1;
    }
    if (len < original_len) {
        oled_default_show_string_with_no_cut(0, y + 1, str, original_len - len); // ?????? ??????
    }
}
/**
 * @brief ?????????????????????? ????????????????????? stable
 * 
 * @param x ?????????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ?????????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param str ????????
 * @param len ????????? ????????????
 */
void oled_default_show_string_with_cut_better(u8 x, u8 y, u8 *str, u8 len) {
    u8 max_row = OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8);
    u8 max_col = OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE;
    u8 default_char_page_size = OLED_DEFAULT_ROW_SIZE / 8;
    u8 i = 0;
    u8 j = 0;
    u8* iter_str = str;
    oled_set_addressing_mode(OLED_PAGE_ADDRESSING_MODE); // ??????????
    if (y >= max_row) {
        return;
    }
    if (x >= max_col) {
        return;
    }
    // ???????????page ?? column
    if (x + len > max_col) {
        len = max_col - x; // ?????????????
    }
    oled_set_page_address_start_column(x * OLED_DEFAULT_COLUMN_SIZE);
    for (; i < default_char_page_size; i++) {
        oled_set_page_address_start_column_and_page(y * default_char_page_size + i, x * OLED_DEFAULT_COLUMN_SIZE); // ????????????
        for (; j < len; j++) {
            oled_write_data(get_char_data_pointer(*iter_str) + i * OLED_DEFAULT_COLUMN_SIZE, OLED_DEFAULT_COLUMN_SIZE);
            iter_str += 1;
        }
        j = 0;
        iter_str = str; // ????????????
    }
}

/**
 * @brief ?????????????????????? ????????????????????? stable
 * ????????????????????????????????????????????????????????????????????
 * @param x ?????????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ?????????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param str ????????
 * @param len ????????? ????????????
 */
void oled_default_show_string_with_no_cut_better(u8 x, u8 y, u8 *str, u8 len) {
    u8 max_row = OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8);
    u8 max_col = OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE;
    u8 default_char_page_size = OLED_DEFAULT_ROW_SIZE / 8;
    u8 i = 0;
    u8 j = 0;
    u8* iter_str;
    u8 original_len = len;
    u8 display_len = 0;
    oled_set_addressing_mode(OLED_PAGE_ADDRESSING_MODE); // ??????????
    if (y >= max_row) {
        return;
    }
    if (x >= max_col) {
        return;
    }
    while (len != 0) {
        
        if (x + len > max_col) {
            len = max_col - x; // ?????????????
        }
        for (i = 0; i < default_char_page_size; i++) { // ????????????
            iter_str = str; // ????????????
            oled_set_page_address_start_column_and_page(y * default_char_page_size + i, x * OLED_DEFAULT_COLUMN_SIZE); // ???????????????????
            for (j = 0; j < len; j++) { // ???????
                oled_write_data(get_char_data_pointer(*iter_str) + i * OLED_DEFAULT_COLUMN_SIZE, OLED_DEFAULT_COLUMN_SIZE);
                iter_str += 1;
            }
        } // ????????????????
        display_len += len; // ???????????????
        str += len; // ???????????
        len = original_len - display_len; // ???????????????
        y += 1; // ????
        x = 0; // ?????????

    }
}
/**
 * @brief ?????????? stable
 * ??????????????????
 * @param x ????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param num ?????????? ?????2147483647
 * @param len ??????????????
 */
void oled_default_show_number_with_cut(u8 x, u8 y, uint32_t num, u8 len) {
    u8 max_row = OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8);
    u8 max_col = OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE;
    if (y >= max_row) {
        return;
    }
    if (x >= max_col) {
        return;
    }
    // ???????????page ?? column
    if (x + len > max_col) {
        len = max_col - x; // ?????????????
    }
    while (len) {
        oled_default_show_char((u8) ((num / Xww_pow(10, len - 1)) % 10) + 48, x++, y);
        len--;
    }
}

/**
 * @brief ?????????? stable
 * ?????????????????????????????
 * @param x ????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param num ?????????? 2147483647
 * @param len ??????????????
 */
void oled_default_show_number_with_no_cut(u8 x, u8 y, uint32_t num, u8 len) {
    u8 max_row = OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8);
    u8 max_col = OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE;
    u8 original_len = len;
    u8 i = 1;
    if (y >= max_row) {
        return;
    }
    if (x >= max_col) {
        return;
    }
    // ???????????page ?? column
    if (x + len > max_col) {
        len = max_col - x; // ?????????????
    }
    
    for (; i <= len; i++) {
        oled_default_show_char((u8) ((num / Xww_pow(10, original_len - i)) % 10) + 48, x++, y);
    }
    if (len < original_len) {
        oled_default_show_number_with_no_cut(0, y + 1, num, original_len - len); // ?????? ??????
    }
}

/**
 * @brief ?????????? ???????? stable
 * ?????????????????????????????
 * @param x ????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param num ?????????? 2147483647
 * @param len ??????????????
 */
void oled_default_show_number_with_cut_better(u8 x, u8 y, uint32_t num, u8 len) { 
    u8 max_row = OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8);
    u8 max_col = OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE;
    u8 default_char_page_size = OLED_DEFAULT_ROW_SIZE / 8;
    u8 original_len = len;
    u8 i = 0;
    u8 j = 0;
    oled_set_addressing_mode(OLED_PAGE_ADDRESSING_MODE); // ??????????
    if (y >= max_row) {
        return;
    }
    if (x >= max_col) {
        return;
    }
    // ???????????page ?? column
    if (x + len > max_col) {
        len = max_col - x; // ?????????????
    }
    oled_set_page_address_start_column(x * OLED_DEFAULT_COLUMN_SIZE);
    for (; i < default_char_page_size; i++) {
        oled_set_page_address_start_column_and_page(y * default_char_page_size + i, x * OLED_DEFAULT_COLUMN_SIZE); // ????????????
        for (j = 0; j < len; j++) {
            oled_write_data(get_char_data_pointer((u8) ((num / Xww_pow(10, original_len - 1 - j)) % 10) + 48) + i * OLED_DEFAULT_COLUMN_SIZE, OLED_DEFAULT_COLUMN_SIZE);
        }
    }
}

/**
 * @brief ?????????? ???????? stable
 * ??????????????????
 * @param x ????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param num ?????????? 2147483647
 * @param len ??????????????
 */
void oled_default_show_number_with_no_cut_better(u8 x, u8 y, uint32_t num, u8 len) { 
    u8 max_row = OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8);
    u8 max_col = OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE;
    u8 default_char_page_size = OLED_DEFAULT_ROW_SIZE / 8;
    u8 i = 0;
    u8 j = 0;
    u8 original_len = len;
    u8 display_len = 0;
    oled_set_addressing_mode(OLED_PAGE_ADDRESSING_MODE); // ??????????
    if (y >= max_row) {
        return;
    }
    if (x >= max_col) {
        return;
    }
    while (len) {
        if (x + len > max_col) {
            len = max_col - x; // ?????????????
        }
        for (i = 0; i < default_char_page_size; i++) { // ????????????
            oled_set_page_address_start_column_and_page(y * default_char_page_size + i, x * OLED_DEFAULT_COLUMN_SIZE); // ???????????????????
            for (j = 0; j < len; j++) { // ???????
                oled_write_data(get_char_data_pointer((u8) ((num / Xww_pow(10, original_len - display_len - 1 - j)) % 10) + 48) + i * OLED_DEFAULT_COLUMN_SIZE, OLED_DEFAULT_COLUMN_SIZE);
            }
        } // ????????????????
        display_len += len; // ?????????????
        len = original_len - display_len; // ?????????????
        y += 1; // ????
        x = 0; // ?????????
    }
}

/**
 * @brief ???????????
 * 
 * @param x ????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param num ??????????
 * @param len ??????????????????????????
 */
void oled_show_number(u8 x, u8 y, int32_t num, u8 len) {
    if (num >= 0) {
        oled_default_show_number_with_no_cut_better(x, y, num, len);
    } else {
        oled_default_show_char('-', x, y);
        oled_default_show_number_with_no_cut_better(x + 1, y, Xww_abs(num), len);
    }
}

/**
 * @brief ???????? stable
 * ??????????????????????????????? ??????????????
 * @param x ????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param str ???????????
 * @param len ????????????????
 */
void oled_show_string(u8 x, u8 y, u8* str, u8 len) { 
    oled_default_show_string_with_no_cut_better(x, y, str, len);
}

/**
 * @brief ????????? stable
 * 
 * @param x ????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param y ????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 * @param num ??????????
 * @param len ???????????????????????
 * @param dot_len ??????????????????
 */
void oled_show_float_number(u8 x, u8 y, float num, u8 len, u8 dot_len) {
    u8 next_row;
    u8 next_col;
    oled_show_number(x, y, (int32_t) num, len);
    // ??????????????????
    get_next_row_and_col(&next_row, &next_col, y, x, len, OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8), OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE);
    // oled_default_show_string_with_cut_better(next_col, next_row, ".", 1);
    oled_default_show_char('.', next_col, next_row);
    get_next_row_and_col(&next_row, &next_col, next_row, next_col, 1, OLED_MAX_PAGE / (OLED_DEFAULT_ROW_SIZE / 8), OLED_MAX_COLUMN / OLED_DEFAULT_COLUMN_SIZE);
    oled_show_number(next_col, next_row, (int32_t) (num * Xww_pow(10, dot_len)), dot_len);
}

/**
 * @brief Get the next row and col?????????
 * 
 * @param next_row ???????????????????????????
 * @param next_col ???????????????????????????
 * @param start_row ????? 
 * @param start_col ??
 * @param display_len ?????????
 * @param max_row ?????
 * @param max_col ?????
 */
void get_next_row_and_col(u8* next_row, u8* next_col, u8 start_row, u8 start_col, u8 display_len, u8 max_row, u8 max_col) { 
    u8 temp = start_col + display_len;
    if (temp >= max_col) { 
        *next_col = temp % max_col;
        *next_row = start_row + temp / max_col;
        if (*next_row >= max_row) {
            *next_row = *next_row % max_row;
        }
    }else {
        *next_col = temp;
        *next_row = start_row;
    }
}

// ???????????????? stable
u8* get_char_data_pointer(u8 chr) {
    return (u8* )OLED_Font[chr - 32];
}


/**
 * @brief ?????????? ???????????????????? stable
 * @param c ???????
 * @param r_len ????
 * @param c_len ????
 * @param start_p_x ????? 0 ~ (128 / OLED_DEFAULT_COLUMN_SIZE) - 1
 * @param start_p_y ????? 0 ~ (64 / OLED_DEFAULT_ROW_SIZE) - 1
 */
void fill_oled_with_char(u8 chr, u8 r_len, u8 c_len, u8 start_p_x, u8 start_p_y, u8 mode) {
    uint16_t length = c_len * r_len;
    // ?????????
    oled_set_addressing_mode(mode);
    // ???????????????
    oled_set_column_address(start_p_x, start_p_x + c_len - 1);
    oled_set_page_address(start_p_y, start_p_y + r_len - 1);
    // ?????????
    OLED_IIC_send_initial_info();
    OLED_IIC_send_byte(OLED_ADDRESS); // ??????????
    OLED_IIC_receive_ack(); // ????ACK
    OLED_IIC_send_byte(OLED_DATA_CONTINUE_MODE); // ?????????? co = 0, D/C# = 1
    OLED_IIC_receive_ack(); // ????ACK
    while (length--) {
        OLED_IIC_send_byte(chr);
        OLED_IIC_receive_ack(); // ????ACK
    }
    OLED_IIC_send_stop();
    reset_column_address();
}

/**
 * @brief ?????OLED stable
 * 
 */
void oled_init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // ????????????? ??????I2C???????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
    u8 init_command[] = {
        OLED_DISPLAY_OFF, // ??????
        OLED_SET_ROW_REVERSE_SCAN_DIRECTION, // ???????????
        OLED_SET_COLUMN_REMAP_REMAP, // ???????????
        OLED_CHARGE_PUMP_ENABLE, // ???????? ????????
        OLED_DISPLAY_ON // ?????
    };
    oled_write_command(init_command, 6);
    clear_oled();
}
