#include "mcp23017.h"

//write function: start, send address, send register, send value, stop.
void setPin(unsigned char address, unsigned char reg, unsigned char value) {
    i2c_master_start();
    i2c_master_send(address & 0xfe); //for writing add 0 to end of address
    i2c_master_send(reg);   //send register
    i2c_master_send(value);
    i2c_master_stop();
}

//read function: start, send address for writing, send register, restart, send address with reading, receive, ack, stop
unsigned char readPin(unsigned char address, unsigned char reg) {
    i2c_master_start();
    i2c_master_send(address & 0xfe);    //for writing add 0 to end of address
    i2c_master_send(reg);   //send register
    i2c_master_restart();
    i2c_master_send(address | 0x01);    //for reading add 1 to end of address
    unsigned char msg = i2c_master_recv();
    i2c_master_ack(1);  //ACK(1) to receive no more bytes
    i2c_master_stop();
    return msg;
}

void clearPin(unsigned char address, unsigned char reg, int pin) {
    unsigned char curr = readPin(address, reg);
    unsigned char val = curr & ~(0x01 << pin);
    setPin(address, reg, val);
}

void mcp_init_forHW3(void) {
    setPin(MCP_ADDRESS, IODIRA_register, 0x00);//    IODIRA = 0x00; make GPA7 output
    setPin(MCP_ADDRESS, IODIRB_register, 0xff);//    IODIRB = 0xFF; make GPB0 input
    setPin(MCP_ADDRESS, OLATA_register, 0x00);//    OLATA = 0x00; set A pins output off first
}