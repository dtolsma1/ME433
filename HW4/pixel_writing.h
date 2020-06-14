#ifndef PIXEL_WRITING_H__
#define PIXEL_WRITING_H__
// Header file for i2c_master_noint.c
// helps implement use I2C1 as a master without using interrupts

#include <xc.h>
#include <stdio.h>
#include "i2c_master_noint.h"
#include "font.h"
#include "ssd1306.h"

void draw_letter(unsigned char x, unsigned char y, unsigned char letter);
void print_array(unsigned char x, unsigned char y, unsigned char* msg_ptr);
void print_variable_value(unsigned char x, unsigned char y, unsigned char* var_msg, unsigned char var);

#endif