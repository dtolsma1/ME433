//HW5
#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include "ws2812b.h"
#include "i2c_master_noint.h"
#include "ssd1306.h"
#include "font.h"
#include "pixel_writing.h"

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0x0000 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

//////////////
void delay();
void heartbeat();

int main() {

    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0; // port A4 is output for LED
    LATAbits.LATA4 = 0; // A4 LED starts off
    TRISBbits.TRISB4 = 1; // port B4 is input from user button
    
    //setup
    i2c_master_setup(); //HW3
    ssd1306_setup();    //HW4
    ws2812b_setup();    //HW5
    
    __builtin_enable_interrupts();
    unsigned char ON = 0xff;
    unsigned char OFF = 0x00;
    
    wsColor colors[NUMLEDS];
    //initial testing
//    ws2812b_setAllSame(colors, ON, OFF, OFF);  
//    ws2812b_initColor(colors, 1, OFF, ON, OFF);
//    ws2812b_initColor(colors, 2, OFF, OFF, ON);
//    ws2812b_initColor(colors, 3, ON, OFF, OFF);
//    ws2812b_initColor(colors, 4, OFF, ON, OFF);
//    ws2812b_setColor(colors, 4);
    //
    
    //rainbow stuffs
    wsColor rainbow_arr[360];
    int i;
    for (i = 0; i < 360; i++) {
        rainbow_arr[i] = HSBtoRGB(i, 1, 0.5);
    }
//    
    int rainbow_index = 0;
    
    //print stuff to ssd1306 for debugging
//    unsigned char var_name[] = "rainbow index";
//    print_variable_value(0, ROW_1, var_name, rainbow_arr[0].r);
//    print_variable_value(10, ROW_2, var_name, colors[0].g);
//    print_variable_value(10, ROW_3, var_name, colors[0].b);
//    print_variable_value(10, ROW_4, var_name, colors[3].b);
    
    print_array(0, ROW_2, "~ r a i n b o w s ~");
    //
    
    while (1) {
        int j;
        for (j = 0; j < NUMLEDS; j++) {
            colors[j] = rainbow_arr[(rainbow_index*20 + j*90) % 360];
        }
        ws2812b_setColor(colors, NUMLEDS);
        
        rainbow_index++;
        
        heartbeat();
//        ssd1306_clear();
//        ssd1306_update();
    }
}

// delay: set count to zero, then while loop
void delay() {
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 24000000/4) {}; // 1 Hz
}

//blink LED and pixel
void heartbeat() {      
        LATAbits.LATA4 = 1; // turn on A4 LED
        ssd1306_drawPixel(1,1,1);
        ssd1306_update();
        delay();
        LATAbits.LATA4 = 0;          // turn off A4 LED
        ssd1306_drawPixel(1,1,0);
        ssd1306_update();
        delay();
}