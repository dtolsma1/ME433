#include "pixel_writing.h"

void draw_letter(unsigned char x, unsigned char y, unsigned char letter) {
//    ASCII[letter - 0x20][j];
    int j = 0;  //0 to 4
    int k = 0;  //0 to 7
    for (k = 0; k < 8; k++) {   
        for (j = 0; j < 5; j++) {   //
            unsigned char pixel_color = (ASCII[letter - 0x20][j] >> k) & 1;
            unsigned char x_loc = x + j;
            unsigned char y_loc = y + k;
            ssd1306_drawPixel(x_loc, y_loc, pixel_color);
        }
    }
    ssd1306_update();
}

void print_array(unsigned char x, unsigned char y, unsigned char* msg_ptr) {
    unsigned char m[50];
    sprintf(m, "%s", msg_ptr);
    int j = 0;
    while (m[j] != 0) {
        unsigned char x_letter_start = x + (5*j);
        draw_letter(x_letter_start, y, m[j]);
        j++;
    }
    ssd1306_update();
}

void print_variable_value(unsigned char x, unsigned char y, unsigned char* var_msg, unsigned char var) {
    unsigned char m[50];
    sprintf(m, "%s = %d", var_msg, var);
    
    int j = 0;
    while (m[j] != 0) {
        unsigned char x_letter_start = x + (5*j);
        draw_letter(x_letter_start, y, m[j]);
        j++;
    }
    
    ssd1306_update();
}
