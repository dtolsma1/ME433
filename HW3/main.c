#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include "i2c_master_noint.h"
#include "mcp23017.h"

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

//////////////HW3
void delay();
void heartbeat();

//unsigned char MCP_ADDRESS = 0b01000000;  // 7bit address of mcp23017, blank R/W bit at end
//unsigned char IODIRA_register = 0x00;
//unsigned char IODIRB_register = 0x01;
//unsigned char OLATA_register = 0x14;
//unsigned char GPIOB_register = 0x13;

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
    
    // always set up first
    //mcp init
    
    i2c_master_setup(); 
    __builtin_enable_interrupts();

    mcp_init_forHW3();
    
    while (1) {
        unsigned char reading = readPin(MCP_ADDRESS, GPIOB_register); //8bits
        if (reading & 0x1) {    // if button not pressed
            clearPin(MCP_ADDRESS, OLATA_register, 7);   // set A7 output off
        } else {    //if pressed
            setPin(MCP_ADDRESS, OLATA_register, (0x01 << 7));
        }
        heartbeat();
    }
}

void delay() {
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 24000000/10) {};   // 24kHz = 1sec
}

void heartbeat() {
        LATAbits.LATA4 = 1; // turn on A4 LED
        delay();
        LATAbits.LATA4 = 0;          // turn off A4 LED
        delay();
}