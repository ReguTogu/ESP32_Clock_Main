#include "image.h"
uint8_t Clock1[8] = {
    0b00111,
    0b01000,
    0b10100,
    0b10010,
    0b10001,
    0b10000,
    0b01000,
    0b00111
};

uint8_t Clock2[8] = {
    0b11000,
    0b00100,
    0b00010,
    0b10010,
    0b00010,
    0b00010,
    0b00100,
    0b11000
};
/*
uint8_t Clock1[8] = {
    0b00111,
    0b01000,
    0b10001,
    0b10001,
    0b10001,
    0b10000,
    0b01000,
    0b00111
};

uint8_t Clock2[8] = {
    0b11100,
    0b00010,
    0b00001,
    0b00001,
    0b11101,
    0b00001,
    0b00010,
    0b11100
};
*/
void config_images() {
    //Create a clock icon
    lcd_create_char(0,Clock1);
    lcd_create_char(1,Clock2);

}

void add_clock_img() {
    lcd_set_cursor(0,0);
    lcd_write(0);
    lcd_set_cursor(1,0);
    lcd_write(1);
}