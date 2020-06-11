//HW2
// generate a 2Hz sine wave & 1Hz triangle wave. Both range from 0-3.3V
//The update rate of the DAC should be at least 50 times faster than the frequency of the signal you are producing.

#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include "spi.h"
#include <proc/p32mx170f256b.h>
#include <math.h>

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
unsigned short assemble_print_message(char c, unsigned short v);
void print_message(unsigned short p);

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
    
    initSPI();
    
    __builtin_enable_interrupts();
    
    //make triangle array
    unsigned short triangle[100];
    int ii; 
    for (ii = 0; ii < 100; ii++) {
        if (ii < 50) {
            triangle[ii] = (unsigned short)(4096/50 * ii);
        } else {
            triangle[ii] = triangle[99-ii];
        }
    }
    
    //make sine wave array
    int jj;
    unsigned short sine[50];
    for (jj = 0; jj < 50; jj++) {
        double radian = (2*3.14159 / 50) * jj;
        sine[jj] = sin(radian) * 2048 + 2048;  //multiply by amplitude, shift up
    }
    
    int triangle_counter = 0;
    int sine_counter = 0;
    
    while (1) {
        unsigned short tri_msg = assemble_print_message('a', triangle[triangle_counter]);
        print_message(tri_msg);
        unsigned short sin_msg = assemble_print_message('b', sine[sine_counter]);
        print_message(sin_msg);
        
        triangle_counter++;
        sine_counter++;
        
        //resets
        if (triangle_counter == 100) {
            triangle_counter = 0;
        }
        if (sine_counter == 50) {
            sine_counter = 0;
        }
        delay();
    }
        
    
}

// initialize SPI1

// delay: set count to zero, then while loop
void delay() {
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 24000000/100) {}; //10ms = 100Hz
}

unsigned short assemble_print_message(char c, unsigned short v) {
    unsigned short p;
    unsigned char cbit;
    if (c == 'a') {
        cbit = 0;
    } else if (c == 'b') {
        cbit = 1;
    } else {
        return;
    }
    p = cbit << 15;
    p = p |(0b111 << 12);
    v = v & 0xfff; // ensure only last 12 bits used
    p = p | v;
    
    return p;
}
 
void print_message(unsigned short p) {
            //write one byte over SPI
        LATAbits.LATA0 = 0; // bring chip select pin (CS) low
        spi_io(p>>8);  // first half //write the byte
        spi_io(p);      // second half
        LATAbits.LATA0 = 1; // bring CS high
}

