#ifndef __OLED_H__
#define __OLED_H__
#include "stm32f10x.h"
#include "math_utils.h"
// ע����GDDRAM�е�ÿһҳ��ÿһ���ֽ��Ǵӵ�λ����λ����


#define OLED_ADDRESS 0x78 // OLED �豸��ַ
// OLED ��������
#define OLED_DATA_CONTINUE_MODE 0x40 // Co = 0, D/C# = 1 ��������ģʽ
#define OLED_COMMAND_CONTINUE_MODE 0x00 // Co = 0, D/C# = 0 ��������ģʽ
#define OLED_DATA_MODE 0xc0 // Co = 1, D/C# = 1 ��һ����ģʽ
#define OLED_COMMAND_MODE 0x80 // Co = 1, D/C# = 0 ��һ����ģʽ


// OLED Ѱַ��ʽ
// ����Ѱַ��ʽ����
#define OLED_SET_ADDRESSING_MODE 0x20 // ���ö�Ӧ��Ѱַ��ʽ֮ǰ�����ȷ��ʹ�����
#define OLED_HORIZONTAL_ADDRESSING_MODE 0x00 // ˮƽѰַģʽ
#define OLED_VERTICAL_ADDRESSING_MODE 0x01 // ��ֱѰַģʽ
#define OLED_PAGE_ADDRESSING_MODE 0x02 // ҳѰַģʽ

void oled_set_addressing_mode(u8 mode);


// ��������ʼ�ͽ���λ������ ��������ˮƽ��ֱѰַģʽ����Ч(����)
#define OLED_SET_COLUMN_ADDRESS 0x21 // ǰ������
// 0-127 �е�ַ ���Խ����ݵĺ���λ��Ч
void oled_set_column_address(u8 start, u8 end);
void reset_column_address(void);


// ����ҳ��ʼ�ͽ���λ������ ��������ˮƽ��ֱѰַģʽ����Ч
#define OLED_SET_PAGE_ADDRESS 0x22 // ǰ������
// 0-7 ҳ��ַ ���Խ����ݵĺ���λ��Ч
void oled_set_page_address(u8 start, u8 end);
void reset_page_address(void);


// ��������ʼλ������ ��������ҳѰַģʽ����Ч
void oled_set_column_start_address_lower_nibble(u8 col); // ��������ʼ��ַ����λ 0000_x3_x2_x1_x0
void oled_set_column_start_address_higher_nibble(u8 col); // ��������ʼ��ַ����λ 0001_0_x2_x1_x0

void oled_set_page_address_start_column_and_page(u8 page, u8 col); // ����ҳ��ַ����ʼ�е�ַ ҳѰַģʽ����Ч
void oled_set_page_address_start_column(u8 col); // ��������ҳѰַģʽ����Ч ������ʼ�е�ַ
// ������ʼҳ��ַ���� ��������ҳѰַģʽ����Ч
void oled_set_start_page_address(u8 page); // ������ʼҳ��ַ 0b1011_0x2x1x0 ҳ��ַ 0-7

// �����е���ӳ�䣨����ָ����GDDRAM��bit����Ļ���صĶ�Ӧ��ϵ��Ĭ�������column address 0 ӳ�䵽 SEG0����ʱ���෴�ģ� �����������ӳ�� �� column address 0 ӳ�䵽 SEG127����ʱ���Ƕ�Ӧ�ģ�
// remapΪ0ʱ����ӳ�� ������ӳ��
// Ĭ��ӳ������
#define OLED_SET_COLUMN_REMAP_DEFAULT 0xA0
// ��ӳ������
#define OLED_SET_COLUMN_REMAP_REMAP 0xA1
void oled_set_column_remap(u8 remap);


// �����е�ɨ�跽�� Ĭ������� ��COM0ɨ�赽COM63���ߵ��� ������ɨ�跽��� ��COM63ɨ�赽COM0
// remapΪ0ʱ������Ĭ�ϵ�ɨ�跽�� �������Ϊ����ɨ��
// Ĭ��ɨ�跽������
#define OLED_SET_ROW_SCAN_DEFAULT_DIRECTION 0xC0 // Com0 - Com63
// ����ɨ������
#define OLED_SET_ROW_REVERSE_SCAN_DIRECTION 0xC8 // Com63 - Com0
void oled_set_row_scan_direction(u8 remap);

// ����OLED��ʾ�Ķ�Ӧ��ϵ
// Ĭ�ϵ���ʾ���� 0 in GDDRAM ��Ӧ�� off on OLED 1 in GDDRAM ��Ӧ�� on on OLED
#define OLED_SET_DISPLAY_MODE_DEFAULT 0xA6
// ������ʾ���� 0 in GDDRAM ��Ӧ�� on on OLED 1 in GDDRAM ��Ӧ�� off on OLED
#define OLED_SET_DISPLAY_MODE_REVERSE 0xA7
void oled_set_display_mod(u8 mode); // mode Ϊ0ʱΪĬ����ʾ ����Ϊ������ʾ

#define OLED_DISPLAY_ON 0xAF // ����ʾ
#define OLED_DISPLAY_OFF 0xAE // �ر���ʾ
void oled_display_on(void);
void oled_display_off(void);


// ���ó���
#define OLED_CHARGE_PUMP_ENABLE 0x8D, 0x14
void oled_enable_charge_pump(void); // ���ó��� ˳��ο��ĵ�


#define OLED_DEFAULT_ROW_SIZE 16 // Ĭ�ϵ��ַ��߶� ������8�ı���
#define OLED_DEFAULT_COLUMN_SIZE 8 // Ĭ�ϵ��ַ�����
#define OLED_MAX_PAGE 8 // OLED ���ҳ��
#define OLED_MAX_COLUMN 128 // OLED �������

/**
 * @brief д��������
 * 
 * @param bytes ����
 * @param length ���ݵĳ���
 */
void oled_write_data(u8* bytes, u8 length);
/**
 * @brief д��������
 * 
 * @param command ����
 * @param length ����ĳ���
 */

void oled_write_command(u8* command, u8 length);

/**
 * @brief OLED ��ʼ��
 * 
 */
void oled_init(void);
void clear_oled(void);
void fill_oled(u8 val);
void fill_oled_with_char(u8 chr, u8 r_len, u8 c_len, u8 start_p_x, u8 start_p_y, u8 mode);
void oled_default_show_char(u8 chr, u8 p_x, u8 p_y);
void oled_write_datas(u8* bytes, u8 r_len, u8 c_len, u8 start_p_x, u8 start_p_y);
u8* get_char_data_pointer(u8 chr);
void oled_default_show_string_with_cut(u8 x, u8 y, u8* str, u8 len);
void oled_default_show_string_with_no_cut(u8 x, u8 y, u8* str, u8 len);
void oled_default_show_string_with_cut_better(u8 x, u8 y, u8* str, u8 len);
void oled_default_show_string_with_no_cut_better(u8 x, u8 y, u8* str, u8 len);
void oled_default_show_number_with_cut(u8 x, u8 y, uint32_t num, u8 len);
void oled_default_show_number_with_no_cut(u8 x, u8 y, uint32_t num, u8 len);
void oled_default_show_number_with_cut_better(u8 x, u8 y, uint32_t num, u8 len);
void oled_default_show_number_with_no_cut_better(u8 x, u8 y, uint32_t num, u8 len);
void oled_show_number(u8 x, u8 y, int32_t num, u8 len);
void oled_show_string(u8 x, u8 y, u8* str, u8 len);
void oled_show_float_number(u8 x, u8 y, float num, u8 len, u8 dot_len);
void get_next_row_and_col(u8* next_row, u8* next_col, u8 start_row, u8 start_col, u8 display_len, u8 max_row, u8 max_col);

#endif // !__OLED_H__
