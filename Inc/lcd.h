/*
 * lcd.h
 *
 *  Created on: Sep 12, 2022
 *      Author: dang khoa
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

void lcd_init(void);
void lcd_write_cmd(char cmd);
void lcd_write_data(char data);
void lcd_clear(void);
void lcd_return_home(void);
void lcd_write_string(char *str);
void lcd_put_cursor(int row, int col);
void lcd_shift(uint8_t RL, uint8_t SC);
void lcd_cursor_display_mode (uint8_t on_off,uint8_t display, uint8_t blink);

#endif /* INC_LCD_H_ */
